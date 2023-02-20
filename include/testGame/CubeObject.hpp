//
// Created by vini84200 on 2/19/23.
//

#ifndef ENTREPORTAIS_CUBEOBJECT_HPP
#define ENTREPORTAIS_CUBEOBJECT_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class CubeObject : public IObject {

    public:
        void CustomImGui() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        CubeObject(char *name);
    };

} // entre_portais

#endif //ENTREPORTAIS_CUBEOBJECT_HPP
