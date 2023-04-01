#ifndef ENTREPORTAIS_CREDITSSCENE_HPP
#define ENTREPORTAIS_CREDITSSCENE_HPP

#include "entrePortaisEngine/IScene.hpp"

namespace labirinto {
    class CreditsScene : public entre_portais::IScene {
    public:
        CreditsScene();

        void update(float deltaTime) override;

        void render() override;

        void initialize() override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;
    };
}


#endif //ENTREPORTAIS_CREDITSSCENE_HPP
