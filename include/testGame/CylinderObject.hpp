//
// Created by vini84200 on 2/19/23.
//

#ifndef ENTREPORTAIS_CYLINDEROBJECT_HPP
#define ENTREPORTAIS_CYLINDEROBJECT_HPP

#include "entrePortaisEngine/IObject.hpp"
#include "entrePortaisEngine/Bezier.hpp"

namespace entre_portais {

    class CylinderObject : public IObject {
    public:
        void CustomImGui() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        CylinderObject(char *name);

        void bezierMove(float t);

        void bezierStart();

        void bezierUpdate();

    private:
        Bezier bezier_;
        bool moving_;
        float t_;
    };

} // entre_portais

#endif //ENTREPORTAIS_CYLINDEROBJECT_HPP
