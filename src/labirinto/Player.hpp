#ifndef ENTREPORTAIS_PLAYER_HPP
#define ENTREPORTAIS_PLAYER_HPP


#include "entrePortaisEngine/Characters/FirstPersonCharacter.hpp"

namespace labirinto {
    class Player : public entre_portais::FirstPersonCharacter {
    public:
        Player(char *name);

        void loadBodyMesh() override;

    private:
        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void render(entre_portais::RenderPass current_pass) override;

    };
} // labirinto


#endif //ENTREPORTAIS_PLAYER_HPP
