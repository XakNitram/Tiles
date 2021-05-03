#pragma once

#include "pch.hpp"
#include "Cell.hpp"

namespace world {
    // If we use unsigned numbers and handle overflow correctly, we can get a circular world.
    constexpr size_t edgeCells = 4;
    constexpr size_t chunkCells = edgeCells * edgeCells;

    using WorldScalar = int32_t;
    using WorldCoordinate = std::tuple<WorldScalar, WorldScalar>;
    using Cells = std::vector<world::Cell const *>;

    class Chunk {
        WorldCoordinate position;
        Cells cells {chunkCells};

    public:
        Chunk() = delete;
        explicit Chunk(WorldCoordinate &position);

        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        Chunk(Chunk&&) = default;
        Chunk& operator=(Chunk&&) = default;

        [[nodiscard]] WorldCoordinate Position() const;

        [[nodiscard]] std::string show() const;
    };
}
