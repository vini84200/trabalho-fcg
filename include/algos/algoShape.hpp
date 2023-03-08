#ifndef ENTREPORTAIS_ALGOSHAPE_HPP
#define ENTREPORTAIS_ALGOSHAPE_HPP

#include "glm/vec3.hpp"

namespace algo::shapes
{
// A 3D Shape
// FONTE: (adaptado) https://github.com/DanielChappuis/reactphysics3d/blob/master/include/reactphysics3d/collision/shapes/ConvexShape.h
    class Shape {
    public:
        glm::vec3 getLocalSupportPointWithMargin(const glm::vec3& direction) const;
        virtual glm::vec3 getLocalSupportPointWithoutMargin(const glm::vec3& direction) const = 0;

        Shape(float margin);
        virtual ~Shape() = default;

        float getMargin() const {
            return margin_;
        }

        void setMargin(float margin) {
            margin_ = margin;
        }

    private:
        float margin_;
    };
}

#endif  // ENTREPORTAIS_ALGOSHAPE_HPP
