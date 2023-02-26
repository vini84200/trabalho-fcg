#include "entrePortaisEngine/physics/collisions.hpp"

namespace entre_portais::collisions {

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionBoxSphere(glm::mat4 &box_transform, glm::mat4 &sphere_transform) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 &box_transform, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionBoxBox(glm::mat4 &box1_t, glm::mat4 &box2_t) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionSphereSphere(glm::mat4 &s1_t, float s1_r, glm::mat4 &s2_t, float s2_r) {
        glm::vec4 s1_center = s1_t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 s2_center = s2_t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        float radii = s1_r + s2_r;
        float distance = glm::length(s1_center - s2_center);
        if (distance > radii) {
            return collisions::PossibleCollision();
        }

        glm::vec3 normal = glm::normalize(glm::vec3(s2_center - s1_center));
        glm::vec3 pointA = glm::vec3(s1_center) + normal * s1_r;
        glm::vec3 pointB = glm::vec3(s2_center) - normal * s2_r;

        return collisions::PossibleCollision(pointA, pointB, normal);
    }
}