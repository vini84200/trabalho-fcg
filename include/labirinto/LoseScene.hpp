#ifndef ENTREPORTAIS_LOSESCENE_HPP
#define ENTREPORTAIS_LOSESCENE_HPP

#include "entrePortaisEngine/IScene.hpp"

namespace labirinto {
    class LoseScene : public entre_portais::IScene {
    public:
        LoseScene();

        void update(float deltaTime) override;

        void render() override;

        void initialize() override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;
    };
}


#endif //ENTREPORTAIS_LOSESCENE_HPP
