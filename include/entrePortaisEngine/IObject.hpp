#ifndef ENTREPORTAIS_IOBJECT_HPP
#define ENTREPORTAIS_IOBJECT_HPP

#include "IGameNode.hpp"
#include "render/IRenderable.hpp"
#include "entrePortaisEngine/meshes/IMesh.hpp"
#include "Transform.hpp"
#include "Logger.hpp"
#include "entrePortaisEngine/physics/RigidBody.hpp"

namespace entre_portais {

    class IObject : public IGameNode, public IRenderable {
        /* Objeto tem um mesh*/
    public:
        IObject(const char *name) : IGameNode(name) {
        }

        ~IObject();;

        IObject(const IObject &other) = delete;

        IObject &operator=(const IObject &other) = delete;

        IObject(IObject &&other) = delete;

        IObject &operator=(IObject &&other) = delete;

        void render(RenderPass current_pass) override;

        void onTransformChange() override;

        glm::mat4 &getModelMatrix() override;

        void renderImGui(bool *p_open = nullptr) override;

        void update(float deltaTime) override = 0;

        void initialize() override = 0;

        void onResize(int width, int height) override = 0;

        std::shared_ptr<IScene> getScene() override;

        void setScene(std::shared_ptr<IScene> scene) override;

        bool hasScene() override;

        void Hide();

        void Show();

        void ToggleVisibility();

        void SetVisibility(bool visible);

        bool IsVisible();

    protected:
        std::shared_ptr<IMesh> mesh_;
        std::weak_ptr<IScene> scene_;
        std::unique_ptr<RigidBody> rigidBody_;
    public:
        const std::shared_ptr<IMesh> &getMesh() const;

        void setMesh(const std::shared_ptr<IMesh> &mesh);

        const std::unique_ptr<RigidBody> &getRigidBody() const;

        void setRigidBody(std::unique_ptr<RigidBody> rigidBody);

    protected:
        bool visible_ = true;
    };

} // entre_portais


#endif //ENTREPORTAIS_IOBJECT_HPP
