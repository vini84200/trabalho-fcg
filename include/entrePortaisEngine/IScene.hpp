#ifndef ENTREPORTAIS_ISCENE_HPP
#define ENTREPORTAIS_ISCENE_HPP

#include <stdexcept>

#include "Camera.hpp"
#include "IGameNode.hpp"
#include "entrePortaisEngine/render/Renderer.hpp"
#include "imgui.h"

namespace entre_portais {
    class IScene : public IGameNode {
        /* Cena exibida, e pode ser trocada.
         * Pode ser uma tela de menu, de jogo, etc. */
    public:
        virtual ~IScene() = default;

        IScene(char *name);

        std::shared_ptr<IScene> getScene() {
            return std::dynamic_pointer_cast<IScene>(shared_from_this());
        }

        // Remove construtores e operadores de cópia e movimentação
        IScene(const IScene &other) = delete;

        IScene &operator=(const IScene &other) = delete;

        IScene(IScene &&other) = delete;

        IScene &operator=(IScene &&other) = delete;

        virtual void initialize() = 0;

        virtual void update(double deltaTime) = 0;

        virtual void render();

        virtual void renderImGui() override;

        virtual void onResize(int width, int height) = 0;

        virtual void onKey(int key, int scancode, int action, int mods) = 0;

        virtual void onMouseButton(int button, int action, int mods) = 0;

        // TODO: Adicionar mais eventos
        virtual void onExit() = 0;

        bool isRoot() override { return true; }

        void setParent(std::shared_ptr<IGameNode> parent) override {
            throw std::runtime_error("Não se pode definir o pai de uma cena, pois essa é a raiz da hierarquia.");
        }

        void setScene(std::shared_ptr<IScene> scene) override {
            throw std::runtime_error("Não se pode definir a cena de uma cena, pois essa é a raiz da hierarquia.");
        }

        bool hasScene() override {
            return true;
        }

        void setCamera(std::shared_ptr<Camera> camera){
            camera_ = camera;
        }

       protected:
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<Renderer> renderer_;

       public:
        const std::shared_ptr<Renderer> &getRenderer() const;
    };

}

#endif //ENTREPORTAIS_ISCENE_HPP
