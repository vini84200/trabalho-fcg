#include "entrePortaisEngine/IObject.hpp"
#include "utils/matrices.h"
#include "entrePortaisEngine/Logger.hpp"


void entre_portais::IObject::render() {
    // Set uniforms
    mesh_->UseShader();
    mesh_->GetShader()->setUniformMat4("model", transform_.getModelMatrix());
    mesh_->GetShader()->setUniformMat4("view", matrices::Matrix_Identity());
    mesh_->GetShader()->setUniformMat4("projection", matrices::Matrix_Identity());
    Draw();
    mesh_->UnbindShader();
}

std::shared_ptr<entre_portais::IScene> entre_portais::IObject::getScene() {
    if (scene_.expired()) {
        log("IObject::getScene() - WARN: Scene is expired");
        return nullptr;
    }
    return scene_.lock();
}

void entre_portais::IObject::setScene(std::shared_ptr<IScene> scene) {
    scene_ = scene;
}

bool entre_portais::IObject::hasScene() {
    return !scene_.expired();
}
