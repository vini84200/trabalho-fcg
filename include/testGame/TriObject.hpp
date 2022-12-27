#ifndef ENTREPORTAIS_TRIOBJECT_HPP
#define ENTREPORTAIS_TRIOBJECT_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {
    class TriObject : public IObject {
    public:
        TriObject();

        ~TriObject() override {
            log("TriObject destroyed");
        }


        TriObject(const TriObject &other) = delete;

        TriObject &operator=(const TriObject &other) = delete;

        TriObject(TriObject &&other) = delete;

        TriObject &operator=(TriObject &&other) = delete;

        void update() override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;
    };

};
#endif //ENTREPORTAIS_TRIOBJECT_HPP
