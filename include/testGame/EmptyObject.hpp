//
// Created by barbu on 11/03/2023.
//

#ifndef ENTREPORTAIS_EMPTYOBJECT_HPP
#define ENTREPORTAIS_EMPTYOBJECT_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class EmptyObject : public IObject {
    public:
        EmptyObject(const char *name) : IObject(name) {}

        ~EmptyObject() override = default;

        EmptyObject(const EmptyObject &other) = delete;

        EmptyObject &operator=(const EmptyObject &other) = delete;

        EmptyObject(EmptyObject &&other) = delete;

        EmptyObject &operator=(EmptyObject &&other) = delete;

        void render() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;
    };

} // entre_portais

#endif //ENTREPORTAIS_EMPTYOBJECT_HPP
