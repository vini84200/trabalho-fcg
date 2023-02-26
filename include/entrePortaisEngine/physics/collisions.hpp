#ifndef ENTREPORTAIS_COLLISIONS_HPP
#define ENTREPORTAIS_COLLISIONS_HPP

#include "glm/glm.hpp"

namespace entre_portais::collisions {

    struct PossibleCollision {
        glm::vec3 pointA;
        glm::vec3 pointB;
        glm::vec3 normal; // Normal of the collision, normalized, pointing from A to B
        bool isColliding;

        PossibleCollision() : isColliding(false) {}

        PossibleCollision reverse() const {
            return PossibleCollision(pointB, pointA, -normal, isColliding);
        }

        PossibleCollision(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 normal) : pointA(pointA), pointB(pointB),
                                                                                  normal(normal), isColliding(true) {}

        PossibleCollision(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 normal, bool isColliding) : pointA(pointA),
                                                                                                    pointB(pointB),
                                                                                                    normal(normal),
                                                                                                    isColliding(
                                                                                                            isColliding) {}

        PossibleCollision(bool isColliding) : isColliding(isColliding) {}

        PossibleCollision(glm::vec3 pointA, glm::vec3 pointB) : pointA(pointA), pointB(pointB), isColliding(true) {
            normal = glm::normalize(pointB - pointA);
        }
    };

    using namespace entre_portais;

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point);


    collisions::PossibleCollision checkCollisionBoxSphere(glm::mat4 &box_transform, glm::mat4 &sphere_transform);


    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 &box_transform, glm::vec4 point);


    collisions::PossibleCollision checkCollisionBoxBox(glm::mat4 &box1_t, glm::mat4 &box2_t);

    collisions::PossibleCollision checkCollisionSphereSphere(glm::mat4 &s1_t, float s1_r, glm::mat4 &s2_t, float s2_r);

}

#endif //ENTREPORTAIS_COLLISIONS_HPP
