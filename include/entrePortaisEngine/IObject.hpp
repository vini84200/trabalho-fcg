#ifndef ENTREPORTAIS_IOBJECT_HPP
#define ENTREPORTAIS_IOBJECT_HPP

#include "IGameNode.hpp"
#include "render/IRenderable.hpp"
#include "IMesh.hpp"
#include "Transform.hpp"
#include "Logger.hpp"

namespace entre_portais {

    class IObject : public IGameNode, public IRenderable {
        /* Objeto tem um mesh*/
    public:
        IObject(char *name) : IGameNode(name) {}

        virtual ~IObject() = default;

        IObject(const IObject &other) = delete;

        IObject &operator=(const IObject &other) = delete;

        IObject(IObject &&other) = delete;

        IObject &operator=(IObject &&other) = delete;

        void Draw() {
            if (mesh_ != nullptr) {
                mesh_->Draw();
            }
        }

        void render() override;

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
        bool visible_ = true;
    };

} // entre_portais


#endif //ENTREPORTAIS_IOBJECT_HPP
