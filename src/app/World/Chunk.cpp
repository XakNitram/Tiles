
#include "pch.hpp"
#include "Chunk.hpp"


namespace world {
    Chunk::Chunk(WorldCoordinate &position) : position(position) {
        for (size_t i = 0; i < chunkCells; i++) {
            cells[i] = world::Cell::byType(world::Cell::Type::Stone);
        }
    }

    std::string Chunk::show() const {
        const size_t ecs = edgeCells - 1;
        const size_t final = chunkCells - 1;
        std::stringstream stream;
        stream << "[";
        for (uint32_t i = 0; i < final; i++) {
            stream << cells[i]->name << "," << ((i & ecs) == ecs ? "\n " : " ");
        }
        stream << cells[final]->name << "]" << std::endl;
        return stream.str();
    }

    WorldCoordinate Chunk::Position() const {
        return position;
    }
}
