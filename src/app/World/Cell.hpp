#pragma once

#include "pch.hpp"

namespace world {
    struct Cell {
        enum class Type {
            Air, Stone, Door, Grate
        };

        const uint32_t id;
        const char *name;
        const uint8_t atlasStart;
        const bool connected;

        const static Cell cells[4];

        // Format of connected textures. Offset to add to the atlasID.
        const static uint8_t connections[256];

        static Cell const *byType(Cell::Type type);

        friend std::ostream &operator<<(std::ostream &os, const Cell &vec);
    };
}
