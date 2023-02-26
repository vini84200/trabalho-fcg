#ifndef ENTREPORTAIS_BOXES_HPP
#define ENTREPORTAIS_BOXES_HPP

#include "glm/vec3.hpp"

namespace entre_portais {
    /**
     * @brief A 3D box, witch is aligned with the world axes. It is used to
     * represent the bounding box of a object.
     */
    struct BoundingBox {
        glm::vec3 min;
        glm::vec3 max;

        BoundingBox() {
            min = glm::vec3(0.0f);
            max = glm::vec3(0.0f);
        };

        BoundingBox(glm::vec3 min, glm::vec3 max) {
            this->min = min;
            this->max = max;
        };

        BoundingBox(glm::vec3 center, float radius) {
            min = center - glm::vec3(radius);
            max = center + glm::vec3(radius);
        };

        bool operator==(const BoundingBox &other) const {
            return min == other.min && max == other.max;
        }

        bool operator!=(const BoundingBox &other) const {
            return !(*this == other);
        }

        bool contains(const BoundingBox &other) const {
            return (other.min.x >= min.x && other.min.y >= min.y && other.min.z >= min.z &&
                    other.max.x <= max.x && other.max.y <= max.x && other.max.z <= max.z);
        }

        bool contains(const glm::vec3 &point) const {
            return (point.x >= min.x && point.y >= min.y && point.z >= min.z &&
                    point.x <= max.x && point.y <= max.x && point.z <= max.z);
        }

        bool intersects(const BoundingBox &other) const {
            return (other.min.x <= max.x && other.min.y <= max.y && other.min.z <= max.z &&
                    other.max.x >= min.x && other.max.y >= min.y && other.max.z >= min.z);
        }
    };
}
#endif //ENTREPORTAIS_BOXES_HPP
