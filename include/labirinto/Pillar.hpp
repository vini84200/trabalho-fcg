#ifndef ENTREPORTAIS_PILLAR_HPP
#define ENTREPORTAIS_PILLAR_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace labirinto {

    class Pillar : public entre_portais::IObject {
    public:
        Pillar(char *path, char *name);

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

    private:
        std::vector<glm::vec3> positions_;
    };

} // labirinto

#endif //ENTREPORTAIS_PILLAR_HPP
