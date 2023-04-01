#ifndef ENTREPORTAIS_HELPSCENE_HPP
#define ENTREPORTAIS_HELPSCENE_HPP

#include "entrePortaisEngine/IScene.hpp"

namespace labirinto {
    class HelpScene : public entre_portais::IScene {
    public:
        HelpScene();

        void update(float deltaTime) override;

        void render() override;

        void initialize() override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;
    };
}


#endif //ENTREPORTAIS_HELPSCENE_HPP
