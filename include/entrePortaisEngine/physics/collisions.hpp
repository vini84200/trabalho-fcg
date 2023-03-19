#ifndef ENTREPORTAIS_COLLISIONS_HPP
#define ENTREPORTAIS_COLLISIONS_HPP

#include <vector>
#include <array>
#include <string>
#include "glm/glm.hpp"
#include "utils/matrices.h"
#include "glm/gtx/norm.hpp"
#include "algos/gjk/Contact.h"

#define MAX_GJK_ITERATIONS 32
namespace entre_portais::collisions {
    struct PossibleCollision {
        bool isColliding;
        std::vector<Contact> contacts;

        PossibleCollision(bool isColliding, std::vector<Contact> contacts) : isColliding(isColliding),
                                                                             contacts(contacts) {}

        PossibleCollision(bool isColliding) : isColliding(isColliding) {};

        PossibleCollision() : isColliding(false) {};

        PossibleCollision reverse() const {
            std::vector<Contact> newContacts;
            for (auto &contact: contacts) {
                newContacts.push_back(contact.reverse());
            }
            return PossibleCollision(isColliding, newContacts);
        }

        PossibleCollision operator-() const {
            std::vector<Contact> newContacts;
            for (auto &contact: contacts) {
                newContacts.push_back(contact.reverseNormal());
            }
            return PossibleCollision(isColliding, newContacts);
        }

        PossibleCollision reversePoints() const {
            std::vector<Contact> newContacts;
            for (auto &contact: contacts) {
                newContacts.push_back(contact.reversePoints());
            }
            return PossibleCollision(isColliding, newContacts);
        }
    };


    using namespace entre_portais;

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point);


    collisions::PossibleCollision
    checkCollisionBoxSphere(glm::mat4 box_transform, const glm::vec3 &box_size, glm::mat4 &sphere_transform,
                            float sphere_radius);


    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 box_transform, glm::vec4 point);


    collisions::PossibleCollision
    checkCollisionBoxBox(const glm::mat4 &box1_t, glm::vec3 &box1_size, const glm::mat4 &box2_t, glm::vec3 &box2_size);

    collisions::PossibleCollision checkCollisionSphereSphere(glm::mat4 &s1_t, float s1_r, glm::mat4 &s2_t, float s2_r);

}

#endif //ENTREPORTAIS_COLLISIONS_HPP
