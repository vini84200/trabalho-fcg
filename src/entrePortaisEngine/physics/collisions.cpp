#include "entrePortaisEngine/physics/collisions.hpp"
#include <vector>

namespace entre_portais::collisions {

    namespace SAT {
        // SEPARATING AXIS THEOREM
        // https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf, pag 5-7

        struct CalculatedRs {
            float r0;
            float r1;
            float r2;
            CalculatedRs(float r0, float r1, float r2) {
                this->r0 = r0;
                this->r1 = r1;
                this->r2 = r2;
            };
        };

        struct Cube {
            glm::vec3 center;
            glm::vec3 axis0;
            glm::vec3 axis1;
            glm::vec3 axis2;
            float extent0;
            float extent1;
            float extent2;
            Cube(glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, float extent0, float extent1, float extent2) {
                this->center = center;
                this->axis0 = axis0;
                this->axis1 = axis1;
                this->axis2 = axis2;
                this->extent0 = extent0;
                this->extent1 = extent1;
                this->extent2 = extent2;
            };
            Cube(glm::mat4 &transform) {
                this->center = glm::vec3(transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                auto axis0 = glm::vec3(transform * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
                auto axis1 = glm::vec3(transform * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                auto axis2 = glm::vec3(transform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
                this->extent0 = axis0.length();
                this->extent1 = axis1.length();
                this->extent2 = axis2.length();
                this->axis0 = axis0 / extent0;
                this->axis1 = axis1 / extent1;
                this->axis2 = axis2 / extent2;
            };
            Cube() {
                this->center = glm::vec3(0.0f);
                this->axis0 = glm::vec3(1.0f, 0.0f, 0.0f);
                this->axis1 = glm::vec3(0.0f, 1.0f, 0.0f);
                this->axis2 = glm::vec3(0.0f, 0.0f, 1.0f);
                this->extent0 = 0.0f;
                this->extent1 = 0.0f;
                this->extent2 = 0.0f;
            };
        };

        struct Precalculations
        {
            Cube cubeA;
            Cube cubeB;
            glm::mat4 C;
            Precalculations(Cube cubeA, Cube cubeB, glm::mat4 C) {
                this->cubeA = cubeA;
                this->cubeB = cubeB;
                this->C = C;
            };
            Precalculations(glm::mat4 &boxA, glm::mat4 &boxB) {
                this->cubeA = Cube(boxA);
                this->cubeB = Cube(boxB);
                this->C = boxB * glm::transpose(boxA);
            };
        };
        


        float projectPointIntoNormalDist(glm::vec3 point, glm::vec3 normal) {
            // normal is a normalized vector
            return glm::dot(point, normal);
        }

        std::vector<glm::vec3> getCubePoints(glm::mat4 &transform) {
            return {
                transform * glm::vec4(-1/2,-1/2,-1/2, 1),
                transform * glm::vec4(1/2,-1/2,-1/2, 1),
                transform * glm::vec4(1/2, 1/2,-1/2, 1),
                transform * glm::vec4(1/2, 1/2,1/2, 1),
                transform * glm::vec4(-1/2, 1/2,1/2, 1),
                transform * glm::vec4(-1/2,-1/2,1/2, 1),
                transform * glm::vec4(1/2,-1/2,1/2, 1),
                transform * glm::vec4(-1/2, 1/2,-1/2, 1)
            };
        }

        CalculatedRs getRsForTest(int i, const Precalculations &precalcs) {
            const Cube &cubeA = precalcs.cubeA;
            const Cube &cubeB = precalcs.cubeB;
            glm::vec3 D = cubeB.center - cubeA.center;

            switch (i)
            {
            case 0:
                return CalculatedRs(
                    cubeA.extent0,
                    cubeB.extent0 * glm::abs(precalcs.C[0][0]) + cubeB.extent1 * glm::abs(precalcs.C[0][1]) + cubeB.extent2 * glm::abs(precalcs.C[0][2]),
                    glm::abs(glm::dot(cubeA.axis0, D))
                );
                break;
            case 1:
                return CalculatedRs(
                    cubeA.extent1,
                    cubeB.extent0 * glm::abs(precalcs.C[1][0]) + cubeB.extent1 * glm::abs(precalcs.C[1][1]) + cubeB.extent2 * glm::abs(precalcs.C[1][2]),
                    glm::abs(glm::dot(cubeA.axis1, D))
                );
                break;
            case 2:
                return CalculatedRs(
                    cubeA.extent2,
                    cubeB.extent0 * glm::abs(precalcs.C[2][0]) + cubeB.extent1 * glm::abs(precalcs.C[2][1]) + cubeB.extent2 * glm::abs(precalcs.C[2][2]),
                    glm::abs(glm::dot(cubeA.axis2, D))
                );
                break;
            case 3:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[0][0]) + cubeA.extent1 * glm::abs(precalcs.C[1][0]) + cubeA.extent2 * glm::abs(precalcs.C[2][0]),
                    cubeB.extent0,
                    glm::abs(glm::dot(cubeB.axis0, D))
                );
                break;
            case 4:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[0][1]) + cubeA.extent1 * glm::abs(precalcs.C[1][1]) + cubeA.extent2 * glm::abs(precalcs.C[2][1]),
                    cubeB.extent1,
                    glm::abs(glm::dot(cubeB.axis1, D))
                );
                break;
            case 5:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[0][2]) + cubeA.extent1 * glm::abs(precalcs.C[1][2]) + cubeA.extent2 * glm::abs(precalcs.C[2][2]),
                    cubeB.extent2,
                    glm::abs(glm::dot(cubeB.axis2, D))
                );
                break;
            case 6:
                return CalculatedRs(
                    cubeA.extent1 * glm::abs(precalcs.C[2][0]) + cubeA.extent2 * glm::abs(precalcs.C[1][0]),
                    cubeB.extent1 * glm::abs(precalcs.C[0][2]) + cubeB.extent2 * glm::abs(precalcs.C[0][1]),
                    glm::abs(glm::dot(cubeA.axis1, D) * precalcs.C[2][0] - glm::dot(cubeA.axis2, D) * precalcs.C[1][0])
                );
                break;
            case 7:
                return CalculatedRs(
                    cubeA.extent1 * glm::abs(precalcs.C[2][1]) + cubeA.extent2 * glm::abs(precalcs.C[1][1]),
                    cubeB.extent0 * glm::abs(precalcs.C[0][2]) + cubeB.extent2 * glm::abs(precalcs.C[0][0]),
                    glm::abs(glm::dot(cubeA.axis1, D) * precalcs.C[2][1] - glm::dot(cubeA.axis2, D) * precalcs.C[1][1])
                );
                break;
            case 8:
                return CalculatedRs(
                    cubeA.extent1 * glm::abs(precalcs.C[2][2]) + cubeA.extent2 * glm::abs(precalcs.C[1][2]),
                    cubeB.extent0 * glm::abs(precalcs.C[0][1]) + cubeB.extent1 * glm::abs(precalcs.C[0][0]),
                    glm::abs(glm::dot(cubeA.axis1, D) * precalcs.C[2][2] - glm::dot(cubeA.axis2, D) * precalcs.C[1][2])
                );
                break;
            case 9:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[2][0]) + cubeA.extent2 * glm::abs(precalcs.C[0][0]),
                    cubeB.extent1 * glm::abs(precalcs.C[1][2]) + cubeB.extent2 * glm::abs(precalcs.C[1][1]),
                    glm::abs(glm::dot(cubeA.axis0, D) * precalcs.C[2][0] - glm::dot(cubeA.axis2, D) * precalcs.C[0][0])
                );
                break;
            case 10:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[2][1]) + cubeA.extent2 * glm::abs(precalcs.C[0][1]),
                    cubeB.extent0 * glm::abs(precalcs.C[1][2]) + cubeB.extent2 * glm::abs(precalcs.C[1][0]),
                    glm::abs(glm::dot(cubeA.axis0, D) * precalcs.C[2][1] - glm::dot(cubeA.axis2, D) * precalcs.C[0][1])
                );
                break;
            case 11:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[2][2]) + cubeA.extent2 * glm::abs(precalcs.C[0][2]),
                    cubeB.extent0 * glm::abs(precalcs.C[1][1]) + cubeB.extent1 * glm::abs(precalcs.C[1][0]),
                    glm::abs(glm::dot(cubeA.axis0, D) * precalcs.C[2][2] - glm::dot(cubeA.axis2, D) * precalcs.C[0][2])
                );
                break;
            case 12:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[1][0]) + cubeA.extent1 * glm::abs(precalcs.C[0][0]),
                    cubeB.extent1 * glm::abs(precalcs.C[2][2]) + cubeB.extent2 * glm::abs(precalcs.C[2][1]),
                    glm::abs(glm::dot(cubeA.axis0, D) * precalcs.C[1][0] - glm::dot(cubeA.axis1, D) * precalcs.C[0][0])
                );
                break;
            case 13:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[1][1]) + cubeA.extent1 * glm::abs(precalcs.C[0][1]),
                    cubeB.extent0 * glm::abs(precalcs.C[2][2]) + cubeB.extent2 * glm::abs(precalcs.C[2][0]),
                    glm::abs(glm::dot(cubeA.axis0, D) * precalcs.C[1][1] - glm::dot(cubeA.axis1, D) * precalcs.C[0][1])
                );
                break;
            case 14:
                return CalculatedRs(
                    cubeA.extent0 * glm::abs(precalcs.C[1][2]) + cubeA.extent1 * glm::abs(precalcs.C[0][2]),
                    cubeB.extent0 * glm::abs(precalcs.C[2][1]) + cubeB.extent1 * glm::abs(precalcs.C[2][0]),
                    glm::abs(glm::dot(cubeA.axis0, D) * precalcs.C[1][2] - glm::dot(cubeA.axis1, D) * precalcs.C[0][2])
                );
                break;
            
            default:
                assert(false);
                return CalculatedRs(0.0f, 0.0f, 0.0f);
                break;
            }
        }
        
    }

    namespace SH {
        // Algorith of Sutherland-Hodgman
        
    }

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
        const SAT::Precalculations precalcs (box1_t, box2_t);
        for (int i = 0; i < 15; i++) {
            SAT::CalculatedRs rs = SAT::getRsForTest(i, precalcs);
            if (rs.r0 > rs.r1 + rs.r2) {
                // No collision
                return collisions::PossibleCollision();
            }
        }

        // Collision
        // TODO: Calculate collision point and normal
        return collisions::PossibleCollision(true);
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

        // TODO: Treat case where they are exacly at the same space as an edge case.

        return collisions::PossibleCollision(pointA, pointB, normal);
    }
}