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
