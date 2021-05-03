#include "pch.hpp"
#include "Rectangle.hpp"

namespace math {
    Rectangle::Rectangle(float x1, float y1, float x2, float y2) : a{x1, y1}, b{x2, y2} {}
    Rectangle::Rectangle(glm::vec2 start, glm::vec2 end) : a(start), b(end) {}

    bool Rectangle::intersects(const Rectangle &other) const {
        return false;
    }

    bool Rectangle::contains(glm::vec2 point) const {
        return !(a.x > point.x || b.x < point.x || a.y > point.y || b.y < point.y);
    }
}
