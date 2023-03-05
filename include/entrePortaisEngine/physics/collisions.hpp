#ifndef ENTREPORTAIS_COLLISIONS_HPP
#define ENTREPORTAIS_COLLISIONS_HPP

#include <vector>
#include <array>
#include <string>
#include "glm/glm.hpp"
#include "utils/matrices.h"
#include "glm/gtx/norm.hpp"

#define MAX_GJK_ITERATIONS 32
namespace entre_portais::collisions {
    struct PossibleCollision {
        enum Collision {
            PENETRATION,
            CONTACT,
            NO_COLLISION
        };

        glm::vec3 pointA;
        glm::vec3 pointB;
        glm::vec3 normal; // Normal of the collision, normalized, pointing from A to B
        float depth;
        Collision collisionType;

        PossibleCollision() : depth(0), collisionType(NO_COLLISION) {};

        PossibleCollision(float depth, glm::vec3 normal) : normal(normal), depth(depth), collisionType(PENETRATION) {};

        PossibleCollision(Collision collisionType) : collisionType(collisionType) {};

        PossibleCollision(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 normal, float depth, Collision collisionType)
                : pointA(pointA), pointB(pointB), normal(normal), depth(depth), collisionType(collisionType) {};

        PossibleCollision reverse() const {
            return {pointB, pointA, -normal, depth, collisionType};
        }

        PossibleCollision(glm::vec3 pointA, glm::vec3 pointB) : pointA(pointA), pointB(pointB), collisionType(CONTACT) {
            normal = glm::normalize(pointB - pointA);
        }

        static PossibleCollision noCollision() {
            return {};
        }

        static PossibleCollision penetration() {
            return {PENETRATION};
        };

        static PossibleCollision penetration(float depth, glm::vec3 normal) {
            return {depth, normal};
        }

        static PossibleCollision penetration(float depth, glm::vec3 normal, glm::vec3 pointA, glm::vec3 pointB) {
            return {pointA, pointB, normal, depth, PENETRATION};
        }

        static PossibleCollision contact(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 normal, float depth) {
            return {pointA, pointB, normal, depth, CONTACT};
        }

        bool isContact() const {
            return collisionType == CONTACT;
        }

        bool isPenetration() const {
            return collisionType == PENETRATION;
        }
    };

    namespace GJK_EPA {
        // Uses the Gilbert-Johnson-Keerthi algorithm to check for collision between two convex shapes
        // and returns the collision data if there is a collision using the EPA algorithm.

        // FONTE: https://github.com/DanielChappuis/reactphysics3d/blob/master/src/collision/narrowphase/GJK/GJKAlgorithm.cpp
        // FONTE: https://github.com/DanielChappuis/reactphysics3d/blob/master/src/collision/narrowphase/GJK/VoronoiSimplex.cpp
        // FONTE: https://github.com/DanielChappuis/reactphysics3d/blob/master/include/reactphysics3d/collision/shapes/ConvexShape.h
        // ESSA PARTE DO CÓDIGO FOI ADAPTADA DO CÓDIGO DO AUTOR CITADO

        // SHAPES

        class GJKCollider {
        public:
            virtual glm::vec3 getLocalSupportPointWithoutMargin(glm::vec3 direction) const = 0;

            glm::vec3 getLocalSupportPoint(glm::vec3 direction) const {
                glm::vec3 supportPoint = getLocalSupportPointWithoutMargin(direction);

                if (margin_ != 0.0f) {

                    glm::vec3 unitVec = glm::vec3(0, -1, 0);
                    if (glm::length2(direction) >
                        std::numeric_limits<float>::epsilon() * std::numeric_limits<float>::epsilon()) {
                        unitVec = glm::normalize(direction);
                    }
                    supportPoint += unitVec * margin_;
                }
                return supportPoint;
            }

        protected:
            float margin_ = 0.0f;

            GJKCollider(float margin) : margin_(margin) {}


        public:
            float getMargin() const {
                return margin_;
            }
        };

        class CubeGJKCollider : public GJKCollider {
        protected:
            glm::vec3 halfExtents;
        public:
            inline glm::vec3 getLocalSupportPointWithoutMargin(glm::vec3 direction) const override {
                glm::vec3 supportPoint;
                supportPoint.x = direction.x > 0 ? 0.5f : -0.5f;
                supportPoint.y = direction.y > 0 ? 0.5f : -0.5f;
                supportPoint.z = direction.z > 0 ? 0.5f : -0.5f;
                return supportPoint;
            }

            CubeGJKCollider(glm::vec3 halfExtents) : halfExtents(halfExtents), GJKCollider(0.0f) {}

            void setMargin(float margin) {
                margin_ = margin;
            }
        };

        class SphereGJKCollider : public GJKCollider {
        public:
            SphereGJKCollider(float radius) : GJKCollider(radius) {}

            inline glm::vec3 getLocalSupportPointWithoutMargin(glm::vec3 direction) const override {
                return glm::vec3(0, 0, 0);
            }


        };

        // VERNOI SIMPLEX
        class VoronoiSimplex {
            glm::vec3 points_[4];
            int numPoints_;
            float barycentricCoordinates_[4];
            float pointsLenght2_[4];
            glm::vec3 supportPointsA_[4];
            glm::vec3 supportPointsB_[4];
            bool recomputeClosestPoint_;
            glm::vec3 closestPoint_;
            glm::vec3 closestSupportPointA_;
            glm::vec3 closestSupportPointB_;
            bool isClosestPointValid_;
            constexpr static float const epsilon = std::numeric_limits<float>::epsilon();
            constexpr static float const epsilon2 = epsilon * epsilon;

        private:
            inline void setBarycentricCoordinates(float a, float b, float c, float d) {
                barycentricCoordinates_[0] = a;
                barycentricCoordinates_[1] = b;
                barycentricCoordinates_[2] = c;
                barycentricCoordinates_[3] = d;
            };

            bool recomputeClosestPoint();

            bool checkClosestPointIsValid() const;

            void computeClosestPointOnSegment(const glm::vec3 &a, const glm::vec3 &b, int &bitUsedVertices,
                                              float &t) const;

            void computeClosestPointOnTriangle(const glm::vec3 &a, const glm::vec3 &b,
                                               const glm::vec3 &c, int &bitsUsedPoints, glm::vec3 &baryCoordsABC) const;

            bool computeClosestPointOnTetrahedron(const glm::vec3 &a, const glm::vec3 &b,
                                                  const glm::vec3 &c, const glm::vec3 &d,
                                                  int &bitsUsedPoints, glm::vec2 &baryCoordsAB, glm::vec2 &baryCoordsCD,
                                                  bool &isDegenerate) const;

            int testOriginOutsideOfPlane(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c,
                                         const glm::vec3 &d) const;

            int mapTriangleUsedVerticesToTetrahedron(int triangleUsedVertices, int first, int second, int third) const;


        public:
            VoronoiSimplex();

            ~VoronoiSimplex() = default;

            bool isFull() const;

            bool isEmpty() const;

            int getSimplex(glm::vec3 *supportPointsA, glm::vec3 *supportPointsB, glm::vec3 *points) const;

            float getMaxLength2() const;

            void addPoint(const glm::vec3 &point, const glm::vec3 &supportPointA, const glm::vec3 &supportPointB);

            void removePoint(int index);

            void reduceSimplex(int bitUsedVertices);

            bool isPointInSimplex(const glm::vec3 &point) const;

            bool isAffinelyDependent() const;

            void backupClosestPoint(glm::vec3 &point);

            void computeClosestPointsOfAandB(glm::vec3 &pointA, glm::vec3 &pointB) const;

            bool computeClosestPoint(glm::vec3 &point);

            int isPointOutsideOfPlane(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c,
                                      const glm::vec3 &d) const;
        };

        inline bool VoronoiSimplex::isFull() const {
            return numPoints_ == 4;
        }

        inline bool VoronoiSimplex::isEmpty() const {
            return numPoints_ == 0;
        }

        inline bool VoronoiSimplex::computeClosestPoint(glm::vec3 &point) {
            bool isValid = recomputeClosestPoint();
            point = closestPoint_;
            return isValid;
        }

        inline bool VoronoiSimplex::checkClosestPointIsValid() const {
            return barycentricCoordinates_[0] >= 0.0f && barycentricCoordinates_[1] >= 0.0f &&
                   barycentricCoordinates_[2] >= 0.0f && barycentricCoordinates_[3] >= 0.0f;
        }

        // GJK Algorithm

        class GJKAlgorithm {
        public:
            enum class GJKResult {
                Separated,
                Penetrating,
                CollideInMargin
            };

            GJKAlgorithm() = default;

            ~GJKAlgorithm() = default;

            PossibleCollision
            testCollision(const GJKCollider &colliderA, const GJKCollider &colliderB, GJKAlgorithm::GJKResult &result,
                          const glm::mat4 &transformA, const glm::mat4 &transformB);

        };

    }


    using namespace entre_portais;

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point);


    collisions::PossibleCollision checkCollisionBoxSphere(glm::mat4 box_transform, glm::mat4 &sphere_transform);


    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 box_transform, glm::vec4 point);


    collisions::PossibleCollision checkCollisionBoxBox(glm::mat4 box1_t, glm::mat4 box2_t);

    collisions::PossibleCollision checkCollisionSphereSphere(glm::mat4 &s1_t, float s1_r, glm::mat4 &s2_t, float s2_r);

}

#endif //ENTREPORTAIS_COLLISIONS_HPP
