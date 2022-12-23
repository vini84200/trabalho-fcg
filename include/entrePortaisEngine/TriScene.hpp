//
// Created by vini84200 on 12/22/22.
//

#ifndef ENTREPORTAIS_TRISCENE_HPP
#define ENTREPORTAIS_TRISCENE_HPP

#include "entrePortaisEngine/IScene.hpp"
#include "Shader.hpp"

namespace entre_portais {

    class TriScene : public IScene {
    public:
        TriScene();

        ~TriScene() override;

        void update() override;

        void render() override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

    private:
        unsigned int VBO, VAO, VBO_COLOR;
        Shader *shader_;
    };

} // entre_portais

#endif //ENTREPORTAIS_TRISCENE_HPP
