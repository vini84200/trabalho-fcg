#include "testGame/Player.hpp"
#include "entrePortaisEngine/meshes/CylinderMesh.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"

namespace entre_portais {
    Player::Player(char *name) : FirstPersonCharacter(name) {

    }

    void Player::loadBodyMesh() {
        mesh_ = std::make_shared<MeshFromObj>("assets/objs/VIVY.obj");
        transform_.setPosition(glm::vec3(2, 2, 2));
        transform_.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        transform_.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    }
} // entre_portais