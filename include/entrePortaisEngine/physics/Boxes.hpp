#ifndef ENTREPORTAIS_BOXES_HPP
#define ENTREPORTAIS_BOXES_HPP

#include "glm/vec3.hpp"

namespace entre_portais {
    /**
     * @brief A 3D box, witch is aligned with the world axes. It is used to
     * represent the bounding box of an object.
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

        BoundingBox(glm::vec3 point) {
            min = point;
            max = point;
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

        void addPoint(const glm::vec3 point) {
            if (point.x < min.x) min.x = point.x;
            if (point.y < min.y) min.y = point.y;
            if (point.z < min.z) min.z = point.z;
            if (point.x > max.x) max.x = point.x;
            if (point.y > max.y) max.y = point.y;
            if (point.z > max.z) max.z = point.z;
        }
    };
}
#endif //ENTREPORTAIS_BOXES_HPP
