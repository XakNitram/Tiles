#pragma once

#include "pch.hpp"
#include "Chunk.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"


namespace world {
    struct CoordinateHash {
        std::size_t operator()(const WorldCoordinate &k) const;
    };

    using ChunkMap = std::unordered_map<WorldCoordinate, Chunk, CoordinateHash>;

    class Landscape {
        class Renderer {
            lwvl::VertexArray arrayBuffer;
            lwvl::ArrayBuffer vertexBuffer;
            lwvl::ArrayBuffer offsetBuffer;
            lwvl::ElementBuffer indexBuffer;
            lwvl::ShaderProgram renderControl;

            uint32_t m_instances = 1;

        public:
            explicit Renderer(float aspect);

            void update(ChunkMap& chunkMap);
            void draw();
        };

        ChunkMap map {};
        Renderer renderer;

        void construct(WorldScalar x, WorldScalar y, size_t index);

    public:
        explicit Landscape(float aspect);

        void show();

        void draw();
    };
}
