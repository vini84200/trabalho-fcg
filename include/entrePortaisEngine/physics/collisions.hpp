#ifndef ENTREPORTAIS_COLLISIONS_HPP
#define ENTREPORTAIS_COLLISIONS_HPP

#include "BoxCollider.hpp"

namespace entre_portais::collisions {

    using namespace entre_portais;

    bool checkCollision(const BoxCollider &box1, const BoxCollider &box2);

    bool checkCollision(const BoxCollider &box, const glm::vec4 &point);
}

#endif //ENTREPORTAIS_COLLISIONS_HPP
