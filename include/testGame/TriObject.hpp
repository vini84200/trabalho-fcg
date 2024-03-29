#ifndef ENTREPORTAIS_TRIOBJECT_HPP
#define ENTREPORTAIS_TRIOBJECT_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {
    class TriObject : public IObject {
    public:
        TriObject(char *name);

        ~TriObject() override {
            logger_.getLogger()->info("Destrutor de TriObject chamado");
        }


        TriObject(const TriObject &other) = delete;

        TriObject &operator=(const TriObject &other) = delete;

        TriObject(TriObject &&other) = delete;

        TriObject &operator=(TriObject &&other) = delete;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void CustomImGui() override;

    private:
        float velocity_ = 10.3f;
        Logger logger_;
    };

};
#endif //ENTREPORTAIS_TRIOBJECT_HPP
