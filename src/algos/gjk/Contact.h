//
// Created by vini84200 on 3/8/23.
//

#ifndef ENTREPORTAIS_CONTACT_H
#define ENTREPORTAIS_CONTACT_H


#include "glm/vec3.hpp"

struct Contact {
    Contact() = default;

    Contact(glm::vec3 normal, float depth, glm::vec3 pointA, glm::vec3 pointB) : normal(-normal), depth(depth),
                                                                                 pointA(pointA), pointB(pointB) {};

    Contact(glm::vec3 normal, glm::vec3 pointA, glm::vec3 pointB) : normal(normal), pointA(pointA), pointB(pointB) {};

    Contact reverse() const {
        return {normal, depth, pointB, pointA};
    }

    Contact reverseNormal() const {
        return {normal, depth, pointA, pointB};
    }

    Contact reversePoints() const {
        return {-normal, depth, pointB, pointA};
    }

public:
    glm::vec3 normal;
    glm::vec3 pointA;
    glm::vec3 pointB;
    float depth;
};


#endif //ENTREPORTAIS_CONTACT_H
