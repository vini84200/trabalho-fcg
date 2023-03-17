#ifndef ENTREPORTAIS_CONTACT_H
#define ENTREPORTAIS_CONTACT_H


#include "glm/vec3.hpp"

struct Contact {
    Contact() = default;

    Contact(glm::vec3 normal, float depth, glm::vec3 pointA, glm::vec3 pointB) : normal(-normal), depth(depth),
                                                                                 pointA(pointA), pointB(pointB) {};

    Contact(glm::vec3 normal, glm::vec3 pointA, glm::vec3 pointB) : normal(-normal), pointA(pointA), pointB(pointB) {};

    Contact reverse() const {
        Contact a = {normal, depth, pointB, pointA};
        a.AccumulatedImpulse = AccumulatedImpulse;
        a.AccumulatedFrictionImpulse = AccumulatedFrictionImpulse;
        return a;
    }

    Contact reverseNormal() const {
        Contact a = {normal, depth, pointA, pointB};
        a.AccumulatedImpulse = AccumulatedImpulse;
        a.AccumulatedFrictionImpulse = AccumulatedFrictionImpulse;
        return a;
    }

    Contact reversePoints() const {
        Contact a = {-normal, depth, pointB, pointA};
        a.AccumulatedImpulse = AccumulatedImpulse;
        a.AccumulatedFrictionImpulse = AccumulatedFrictionImpulse;
        return a;
    }

public:
    glm::vec3 normal;
    glm::vec3 pointA;
    glm::vec3 pointB;
    float depth;
    float AccumulatedImpulse = 0;
    float AccumulatedFrictionImpulse = 0;
};


#endif //ENTREPORTAIS_CONTACT_H
