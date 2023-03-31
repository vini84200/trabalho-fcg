#ifndef ENTREPORTAIS_MENUSCENE_HPP
#define ENTREPORTAIS_MENUSCENE_HPP

#include "entrePortaisEngine/IScene.hpp"

namespace labirinto {
    class MenuScene : public entre_portais::IScene {
    public:
        MenuScene();

        void update(float deltaTime) override;

        void render() override;

        void initialize() override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;
    };
}


#endif //ENTREPORTAIS_MENUSCENE_HPP
