#ifndef ENTREPORTAIS_TRISCENE_HPP
#define ENTREPORTAIS_TRISCENE_HPP

#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/render/Shader.hpp"
#include "entrePortaisEngine/render/VertexArrayBuffer.hpp"
#include "entrePortaisEngine/Logger.hpp"
#include "math.h"

namespace entre_portais {

    class TriScene : public IScene {
    public:
        TriScene();

        ~TriScene() override;

        // Remove construtores e operadores de cópia e movimentação
        TriScene(const TriScene &other) = delete;

        TriScene &operator=(const TriScene &other) = delete;

        TriScene(TriScene &&other) = delete;

        TriScene &operator=(TriScene &&other) = delete;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

    private:
        std::shared_ptr<VertexArrayBuffer> vao_ = nullptr;
        Shader *shader_;
        Logger logger_ = Logger("TriScene");
    };

} // entre_portais

#endif //ENTREPORTAIS_TRISCENE_HPP
