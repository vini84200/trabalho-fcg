#ifndef ENTREPORTAIS_OBJFROMFILE_HPP
#define ENTREPORTAIS_OBJFROMFILE_HPP

#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    class ObjFromFile : public IObject {

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

        const char *fileName_;
    public:
        ObjFromFile(const char *name, const char *fileName);
    };

} // entre_portais

#endif //ENTREPORTAIS_OBJFROMFILE_HPP
