#include "entrePortaisEngine/physics/collisions.hpp"
#include "spdlog/spdlog.h"
#include "glm/gtx/string_cast.hpp"
#include "algos/gjk/gjk.h"
#include "algos/sat/SatAlgorithm.hpp"
#include <vector>

#define GTK_EPA_MAX_ITER 32
namespace entre_portais::collisions {

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision
    checkCollisionBoxSphere(glm::mat4 box_transform, const glm::vec3 &box_size, glm::mat4 &sphere_transform,
                            float sphere_radius) {
        auto gjk = reactphysics3d::GJKAlgorithm();
        algo::shapes::SphereShape sphere(sphere_radius);
        algo::shapes::BoxShape box(box_size / 2.0f);
        reactphysics3d::GJKAlgorithm::GJKResult result;
        std::vector<Contact> contacts;
        contacts.reserve(4);
        gjk.testCollision(sphere, sphere_transform, box, box_transform, result, contacts);
        bool isColliding = false;

        if (result == reactphysics3d::GJKAlgorithm::GJKResult::COLLIDE_IN_MARGIN) {
            isColliding = true;
        } else if (result == reactphysics3d::GJKAlgorithm::GJKResult::INTERPENETRATE) {
            reactphysics3d::SATAlgorithm sat(false);
            isColliding |= sat.testCollisionSphereVsConvexPolyhedron(sphere, sphere_transform, box, box_transform,
                                                                     contacts);
        } else if (result == reactphysics3d::GJKAlgorithm::GJKResult::SEPARATED) {
            return collisions::PossibleCollision();
        }
        return collisions::PossibleCollision(isColliding, contacts);

    }

    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 box_transform, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision
    checkCollisionBoxBox(const glm::mat4 &box1_t, glm::vec3 &box1_size, const glm::mat4 &box2_t, glm::vec3 &box2_size) {
        algo::shapes::BoxShape box1(box1_size / 2.f);
        algo::shapes::BoxShape box2(box2_size / 2.f);
        std::vector<Contact> c;
        auto sat = reactphysics3d::SATAlgorithm(true);
        bool isColliding = sat.testCollisionConvexPolyhedronVsConvexPolyhedron(box1, box1_t, box2, box2_t, c);
        return collisions::PossibleCollision(isColliding, c);
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
        float penetration = radii - distance;
        if (isnanf(normal.x) || isnanf(normal.y) || isnanf(normal.z)) {
            spdlog::warn("Normal is nan: {}", glm::to_string(normal));
//            return collisions::PossibleCollision::contact(s1_center, s2_center, glm::vec3(0, 1, 0), penetration);
            return collisions::PossibleCollision(true,
                                                 {Contact(glm::vec3(0, 1, 0), penetration, s1_center, s2_center)});
        }

        // TODO: Treat case where they are exacly at the same space as an edge case.

        if (penetration < 0.f) {
            return collisions::PossibleCollision();
        } else if (glm::length2(normal) < std::numeric_limits<float>::epsilon()) {
            return collisions::PossibleCollision();
        } else {
            return collisions::PossibleCollision(true, {Contact(normal, penetration, s1_center, s2_center)});
        }
    }

}
