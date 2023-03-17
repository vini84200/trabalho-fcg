#ifndef ENTREPORTAIS_BEZIER_HPP
#define ENTREPORTAIS_BEZIER_HPP

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"

namespace entre_portais {

    class Bezier {
    public:
        Bezier() {};

        Bezier(std::vector<glm::vec3> positions, std::vector<glm::quat> rotations);

        void addPosition(glm::vec3 position);

        void addRotation(glm::quat rotation);

        void add(glm::vec3 position, glm::quat rotation);

        glm::vec3 cubicPositionBezierCurve(float t);

        glm::quat cubicRotationBezierCurve(float t);

        int getVectorSize();

        glm::vec3 getLastPos();

        glm::quat getLastRot();

    private:
        std::vector<glm::vec3> positions_;
        std::vector<glm::quat> rotations_;
    };

} // entre_portais

#endif //ENTREPORTAIS_BEZIER_HPP
