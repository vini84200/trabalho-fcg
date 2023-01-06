#ifndef ENTREPORTAIS_TRISCENE_HPP
#define ENTREPORTAIS_TRISCENE_HPP

#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/Shader.hpp"
#include "entrePortaisEngine/VertexArrayBuffer.hpp"
#include "entrePortaisEngine/Logger.hpp"

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

        void update(double deltaTime) override;

        void render() override;

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
