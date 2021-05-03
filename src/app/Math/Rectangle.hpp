#pragma once
#include "pch.hpp"
#include <glm/vec2.hpp>

namespace math {
    class Rectangle {
        glm::vec2 a, b;
    public:
        Rectangle(glm::vec2 start, glm::vec2 end);
        Rectangle(float x1, float y1, float x2, float y2);

        [[nodiscard]] bool intersects(const Rectangle& other) const;
        [[nodiscard]] bool contains(glm::vec2 point) const;
    };
}
