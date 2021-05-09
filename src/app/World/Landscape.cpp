#include "pch.hpp"
#include "Landscape.hpp"

namespace world {
    std::size_t CoordinateHash::operator()(const WorldCoordinate &k) const {
        // Test this.
        const auto a = static_cast<uint64_t>(std::get<0>(k));
        const auto b = static_cast<uint64_t>(std::get<1>(k));

        // Positive integers only.
        //return a >= b ? a * a + b : a + b * b;

        // Positive and negative integers.
        // This appears to be more efficient than the positive only version.
        const auto la = static_cast<int64_t>(a);
        const auto lb = static_cast<int64_t>(b);
        auto A = static_cast<uint64_t>(a >= 0 ? 2 * la : -2 * la - 1);
        auto B = static_cast<uint64_t>(b >= 0 ? 2 * lb : -2 * lb - 1);
        auto C = static_cast<int64_t>((A >= B ? A * A + A + B : A + B * B) / 2);
        return a < 0 && b < 0 || a >= 0 && b >= 0 ? C : -C - 1;

        // Faster but not a very good hash function.
        //return (static_cast<uint64_t>(std::get<0>(k)) << 32) + static_cast<uint64_t>(std::get<1>(k));
    }

    Landscape::Renderer::Renderer(float aspect) {
        const auto chunkSize = static_cast<float>(edgeCells);
        float width, height, xPad, yPad;
        if (aspect >= 1.0f) {
            width = aspect;
            height = 1.0f;

            xPad = -1.0f / aspect;
            yPad = -1.0f;
        } else {
            width = 1.0f;
            height = 1.0f / aspect;

            xPad = -1.0;
            yPad = -aspect;
        }

        lwvl::VertexShader vs{lwvl::VertexShader::readFile("Data/Shaders/chunk.vert")};
        lwvl::FragmentShader fs{lwvl::FragmentShader::readFile("Data/Shaders/chunk.frag")};
        renderControl.link(vs, fs);
        renderControl.bind();

        const float world[16]{
            2.0f / (width * chunkSize), 0.0f, 0.0f, xPad,
            0.0f, 2.0f / (height * chunkSize), 0.0f, yPad,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        };
        renderControl.uniform("projection").setMatrix4(world);

        constexpr uint64_t numQuadsX = edgeCells;
        constexpr uint64_t numQuadsY = edgeCells;
        constexpr uint64_t numVerticesX = numQuadsX + 1;
        constexpr uint64_t numVerticesY = numQuadsY + 1;

        // Construct vertex buffer
        std::array<float, numVerticesX * numVerticesY * 2> chunkVertices{};
        for (uint64_t row = 0; row < numVerticesY; row++) {
            for (uint64_t col = 0; col < numVerticesX; col++) {
                size_t index = size_t((row * numVerticesX + col)) * 2;
                chunkVertices[index + 0] = static_cast<float>(col);
                chunkVertices[index + 1] = static_cast<float>(row);
            }
        }

        arrayBuffer.bind();
        vertexBuffer.bind();
        vertexBuffer.construct(chunkVertices.begin(), chunkVertices.end());
        arrayBuffer.attribute(2, GL_FLOAT, 2 * sizeof(float), 0);

        // Construct offset buffer
        offsetBuffer.bind();
        offsetBuffer.construct<float>(nullptr, 9 * 2 * sizeof(float));
        arrayBuffer.attribute(2, GL_FLOAT, 2 * sizeof(float), 0, 1);

        // Construct index buffer
        std::array<uint32_t, numQuadsX * numQuadsY * 4> chunkIndices{};
        for (uint64_t row = 0; row < numQuadsY; row++) {
            for (uint64_t col = 0; col < numQuadsX; col++) {
                const uint64_t currentQuad = row * numQuadsX + col;
                const uint64_t leftCorner = currentQuad + row;
                const size_t index = size_t(currentQuad) * 4;

                chunkIndices[index + 0] = static_cast<uint32_t>(leftCorner);
                chunkIndices[index + 1] = static_cast<uint32_t>(leftCorner + 1);
                chunkIndices[index + 2] = static_cast<uint32_t>(leftCorner + numVerticesX + 1);
                chunkIndices[index + 3] = static_cast<uint32_t>(leftCorner + numVerticesX);
            }
        }

        indexBuffer.bind();
        indexBuffer.construct(chunkIndices.begin(), chunkIndices.end());

        arrayBuffer.instances(m_instances);

        lwvl::VertexArray::clear();
        lwvl::ArrayBuffer::clear();
        lwvl::ElementBuffer::clear();
    }

    void Landscape::Renderer::draw() {
        renderControl.bind();
        arrayBuffer.bind();
        arrayBuffer.drawElements(
            lwvl::PrimitiveMode::TriangleFan,
            4, lwvl::ByteFormat::UnsignedInt
        );
    }

    void Landscape::Renderer::update(ChunkMap &chunkMap) {
        std::array<float, 9 * 2> offsetData {};
        size_t index = 0;
        const auto chunkSize = static_cast<float>(chunkCells);
        for (const auto&[key, chunk] : chunkMap) {
            const auto[x, y] = chunk.Position();
            offsetData[index + 0] = chunkSize * float(x);
            offsetData[index + 1] = chunkSize * float(y);
            index += 2;
        }

        offsetBuffer.bind();
        offsetBuffer.construct(offsetData.begin(), offsetData.end());
    }

    void Landscape::construct(WorldScalar x, WorldScalar y, size_t index) {
        WorldCoordinate position = std::make_tuple(x, y);
        Chunk chunk {position};
        std::pair<WorldCoordinate, Chunk> value {position, std::move(chunk)};
        map.emplace(std::move(value));
    }

    Landscape::Landscape(float aspect) : renderer(aspect) {
        map.reserve(1);
        //for (int32_t i = -1; i < 2; i++) {
        //    for (int32_t j = -1; j < 2; j++) {
        //        construct(i, j, (i + 1) * 3 + (j + 1));
        //    }
        //}

        renderer.update(map);
    }

    void Landscape::show() {
        for (const auto&[key, chunk] : map) {
            std::cout << std::get<0>(key) << ", " << std::get<1>(key) << ": " << std::endl;
            std::cout << chunk.show() << std::endl;
        }
    }

    void Landscape::draw() {
        renderer.draw();
    }
}
