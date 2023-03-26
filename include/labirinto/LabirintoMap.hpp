//
// Created by vini84200 on 3/26/23.
//

#ifndef ENTREPORTAIS_LABIRINTOMAP_HPP
#define ENTREPORTAIS_LABIRINTOMAP_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace labirinto {

    class LabirintoMap : public entre_portais::IObject {
    public:
        LabirintoMap();

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;
    };

} // labirinto

#endif //ENTREPORTAIS_LABIRINTOMAP_HPP
