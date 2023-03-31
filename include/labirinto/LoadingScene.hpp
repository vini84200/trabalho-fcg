#ifndef ENTREPORTAIS_LOADINGSCENE_HPP
#define ENTREPORTAIS_LOADINGSCENE_HPP

#include "entrePortaisEngine/IScene.hpp"

namespace labirinto {

    class LoadingScene : public entre_portais::IScene {
    public:
        LoadingScene();

        void update(float deltaTime) override;

        void render() override;

        void initialize() override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

    private:
    };

} // labirinto

#endif //ENTREPORTAIS_LOADINGSCENE_HPP
