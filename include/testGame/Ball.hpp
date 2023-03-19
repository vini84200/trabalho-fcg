//
// Created by vini84200 on 2/26/23.
//

#ifndef ENTREPORTAIS_BALL_HPP
#define ENTREPORTAIS_BALL_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class Ball : public IObject {
    public:
        explicit Ball(const char *name);

    private:

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

    };

} // entre_portais

#endif //ENTREPORTAIS_BALL_HPP
