#ifndef ENTREPORTAIS_CUELHO_HPP
#define ENTREPORTAIS_CUELHO_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class Cuelho : public IObject {
    public:
        Cuelho(char *name);

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

    public:
        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

    };

} // entre_portais

#endif //ENTREPORTAIS_CUELHO_HPP
