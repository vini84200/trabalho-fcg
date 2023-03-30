//
// Created by vini84200 on 3/26/23.
//

#ifndef ENTREPORTAIS_GAMESCENE_HPP
#define ENTREPORTAIS_GAMESCENE_HPP

#include "entrePortaisEngine/IScene.hpp"

namespace labirinto {

    class GameScene : public entre_portais::IScene {

    public:
        GameScene();

        void update(float deltaTime) override;

        void render() override;

        void initialize() override;

    private:
        void CustomImGui() override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;
    };

} // labirinto

#endif //ENTREPORTAIS_GAMESCENE_HPP
