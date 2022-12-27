#ifndef ENTREPORTAIS_IOBJECT_HPP
#define ENTREPORTAIS_IOBJECT_HPP

#include "IGameNode.hpp"
#include "IMesh.hpp"
#include "Transform.hpp"

namespace entre_portais {

    class IObject : public IGameNode {
        /* Objeto tem um mesh*/
    public:
        IObject() = default;

        virtual ~IObject() = default;

        IObject(const IObject &other) = delete;

        IObject &operator=(const IObject &other) = delete;

        IObject(IObject &&other) = delete;

        IObject &operator=(IObject &&other) = delete;

        void Draw() {
            mesh_->Draw();
        }

        void render() override;

        void update() override = 0;

        void initialize() override = 0;

        void onResize(int width, int height) override = 0;

        Transform *getTransform() {
            return &transform_;
        }

    protected:
        std::shared_ptr<IMesh> mesh_;
        Transform transform_;
    };

} // entre_portais


#endif //ENTREPORTAIS_IOBJECT_HPP
