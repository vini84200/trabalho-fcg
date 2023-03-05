#ifndef ENTREPORTAIS_COLLISIONS_HPP
#define ENTREPORTAIS_COLLISIONS_HPP

#include <vector>
#include <array>
#include <string>
#include "glm/glm.hpp"
#include "utils/matrices.h"

#define MAX_GJK_ITERATIONS 32
namespace entre_portais::collisions {
    class PossibleCollision;

    namespace GJK_EPA {
        // Uses the Gilbert-Johnson-Keerthi algorithm to check for collision between two convex shapes
        // and returns the collision data if there is a collision using the EPA algorithm.

        // FONTE: https://blog.winter.dev/2020/gjk-algorithm/
        // FONTE: https://blog.winter.dev/2020/epa-algorithm/
        // ESSA PARTE DO CÓDIGO FOI ADAPTADA DO CÓDIGO DO AUTOR ACIMA CITADO

        struct Simplex {
        private:
            std::array<glm::vec3, 4> points_;
            int size_;
        public:
            Simplex()
                    : points_{glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0)},
                      size_(0) {}

            Simplex &operator=(std::initializer_list<glm::vec3> list) {
                size_ = 0;
                for (auto &point: list) {
                    points_[size_++] = point;
                }
                return *this;
            }

            void add(glm::vec3 point) {
                points_[size_++] = point;
            }

            void push_front(glm::vec3 point) {
                points_[3] = points_[2];
                points_[2] = points_[1];
                points_[1] = points_[0];
                points_[0] = point;
                size_ = std::min(size_ + 1, 4);
            }

            glm::vec3 &operator[](int index) {
                return points_[index];
            }

            int getSize() {
                return size_;
            }

            void clear() {
                size_ = 0;
            }

            auto begin() const {
                return points_.begin();
            }

            auto end() const {
                return points_.end() - (4 - size_);
            }

            std::string toString() const;
        };

        class GJKCollider {
        public:
            GJKCollider() = default;

            virtual ~GJKCollider() = default;

            virtual glm::vec3 findFurtherPoint(glm::vec3 direction) const = 0;

            virtual glm::vec3 findSupportPoint(const GJKCollider &other, const glm::vec3 direction) const;
        };

        class CubeGJKCollider : public GJKCollider {

            glm::vec3 findFurtherPoint(glm::vec3 direction) const override;

            glm::mat4 transform;
            std::array<glm::vec3, 8> vertices{};
            glm::vec3 halfExtents;
            glm::vec3 globalCenter;
        public:
            CubeGJKCollider(glm::mat4 &transform) : transform(transform) {
                // Create the vertices of the cube
                glm::vec3 size_ = glm::vec3(1.0f);
                std::vector<glm::vec4> cubeCorners = {
                        {-size_.x, -size_.y, -size_.z, 1.0f},
                        {size_.x,  -size_.y, -size_.z, 1.0f},
                        {size_.x,  size_.y,  -size_.z, 1.0f},
                        {-size_.x, size_.y,  -size_.z, 1.0f},
                        {-size_.x, -size_.y, size_.z,  1.0f},
                        {size_.x,  -size_.y, size_.z,  1.0f},
                        {size_.x,  size_.y,  size_.z,  1.0f},
                        {-size_.x, size_.y,  size_.z,  1.0f}
                };
                for (int i = 0; i < 8; i++) {
                    vertices[i] = transform * cubeCorners[i];
                }
                halfExtents = transform * glm::vec4(1, 1, 1, 0);
                globalCenter = transform * glm::vec4(0, 0, 0, 1);

            }

        };

        class SphereGJKCollider : public GJKCollider {

            glm::vec3 findFurtherPoint(glm::vec3 direction) const override;

            glm::mat4 transform;
            glm::vec3 center_;
            glm::vec3 scaledRadius_;
        public:
            SphereGJKCollider(glm::mat4 &transform) : transform(transform) {
                center_ = transform * glm::vec4(0, 0, 0, 1);
                glm::mat4 scale = matrices::ScalingMatrixFromMatrix(transform);
                scaledRadius_ = scale * glm::normalize(glm::vec4(1, 1, 1, 0));
            }

            ~SphereGJKCollider() override = default;
        };

        PossibleCollision gjk(const GJKCollider &a, const GJKCollider &b);

        PossibleCollision epa(const GJKCollider &a, const GJKCollider &b, const Simplex &simplex);
    }

    struct PossibleCollision {
        glm::vec3 pointA;
        glm::vec3 pointB;
        glm::vec3 normal; // Normal of the collision, normalized, pointing from A to B
        float depth;
        bool isColliding;

        PossibleCollision() : isColliding(false) {};

        PossibleCollision(float depth, glm::vec3 normal) : normal(normal), depth(depth), isColliding(true) {};

        PossibleCollision(float depth, glm::vec3 normal, bool isColliding) : normal(normal), depth(depth),
                                                                             isColliding(isColliding) {};

        PossibleCollision(bool isColliding) : isColliding(isColliding) {};

        PossibleCollision reverse() const {
            return {depth, -normal, isColliding};
        }

        PossibleCollision(glm::vec3 pointA, glm::vec3 pointB) : pointA(pointA), pointB(pointB), isColliding(true) {
            normal = glm::normalize(pointB - pointA);
        }

    };

    using namespace entre_portais;

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point);


    collisions::PossibleCollision checkCollisionBoxSphere(glm::mat4 box_transform, glm::mat4 &sphere_transform);


    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 box_transform, glm::vec4 point);


    collisions::PossibleCollision checkCollisionBoxBox(glm::mat4 box1_t, glm::mat4 box2_t);

    collisions::PossibleCollision checkCollisionSphereSphere(glm::mat4 &s1_t, float s1_r, glm::mat4 &s2_t, float s2_r);

}

#endif //ENTREPORTAIS_COLLISIONS_HPP
