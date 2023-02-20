//
// Created by vini84200 on 2/20/23.
//

#ifndef ENTREPORTAIS_GROUND_HPP
#define ENTREPORTAIS_GROUND_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class Ground : public IObject {
    public:
        Ground();

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void initialize() override;

        void update(float deltaTime) override;

        void onResize(int width, int height) override;
    };

} // entre_portais

#endif //ENTREPORTAIS_GROUND_HPP
