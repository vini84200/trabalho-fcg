#ifndef ENTREPORTAIS_TEAPOTOBJECT_HPP
#define ENTREPORTAIS_TEAPOTOBJECT_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class TeapotObject : public IObject {
    public:
        TeapotObject(char *name);

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

    public:
        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

    };

} // entre_portais

#endif //ENTREPORTAIS_TEAPOTOBJECT_HPP
