#include "entrePortaisEngine/Objects/ObjFromFile.hpp"
#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "entrePortaisEngine/IScene.hpp"

namespace entre_portais {
    void ObjFromFile::onKey(int key, int scancode, int action, int mods) {

    }

    void ObjFromFile::onMouseButton(int button, int action, int mods) {

    }

    void ObjFromFile::onExit() {

    }

    void ObjFromFile::update(float deltaTime) {
    }

    void ObjFromFile::initialize() {
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong");
        submit(renderer);
    }

    void ObjFromFile::onResize(int width, int height) {
    }

    ObjFromFile::ObjFromFile(const char *name, const char *fileName)
            : IObject(name), fileName_(fileName) {
        mesh_ = std::make_shared<MeshFromObj>(fileName_);
    }
} // entre_portais