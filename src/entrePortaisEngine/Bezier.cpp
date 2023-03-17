#include "entrePortaisEngine/Bezier.hpp"

namespace entre_portais {
    void Bezier::addPosition(glm::vec3 position) {
        positions_.push_back(position);
    }

    void Bezier::addRotation(glm::quat rotation) {
        rotations_.push_back(rotation);
    }

    void Bezier::add(glm::vec3 position, glm::quat rotation) {
        addPosition(position);
        addRotation(rotation);
    }

    glm::vec3 Bezier::cubicPositionBezierCurve(float t) {
        int tInt = static_cast<int> (t);
        t -= floor(t);
        float tQuad = t * t;
        float tCube = tQuad * t;
        tInt *= 3;
        glm::vec3 result = positions_.at(tInt) * (-tCube + 3 * tQuad - 3 * t + 1) +
                           positions_.at(tInt + 1) * (3 * tCube - 6 * tQuad + 3 * t) +
                           positions_.at(tInt + 2) * (-3 * tCube + 3 * tQuad) +
                           positions_.at(tInt + 3) * (tCube);
        return result;
    }

    glm::quat Bezier::cubicRotationBezierCurve(float t) {
        int tInt = static_cast<int> (t);
        t -= floor(t);
        float tQuad = t * t;
        float tCube = tQuad * t;
        tInt *= 3;
        glm::quat A = glm::slerp(rotations_.at(tInt), rotations_.at(tInt + 1), t);
        glm::quat B = glm::slerp(rotations_.at(tInt + 1), rotations_.at(tInt + 2), t);
        glm::quat C = glm::slerp(rotations_.at(tInt + 2), rotations_.at(tInt + 3), t);
        glm::quat D = glm::slerp(A, B, t);
        glm::quat E = glm::slerp(B, C, t);
        glm::quat result = glm::slerp(D, E, t);
        return result;
    }

    int Bezier::getVectorSize() {
        return positions_.size();
    }

    glm::vec3 Bezier::getLastPos() {
        return positions_.at(getVectorSize() - 1);
    }

    glm::quat Bezier::getLastRot() {
        return rotations_.at(getVectorSize() - 1);
    }

    Bezier::Bezier(std::vector<glm::vec3> positions, std::vector<glm::quat> rotations) {
        positions_ = positions;
        rotations_ = rotations;
    }
} // entre_portais