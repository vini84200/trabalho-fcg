#ifndef ENTREPORTAIS_IGAMENODE_HPP
#define ENTREPORTAIS_IGAMENODE_HPP

#include <memory>
#include <utility>
#include <vector>

namespace entre_portais {

    class IScene;

    class IGameNode : public std::enable_shared_from_this<IGameNode> {
        /* Objeto que participa da hierarquia na renderização e
         * propagação de eventos.*/

    public:
        IGameNode(char *name) : name_(name) {}

        virtual ~IGameNode() = default;

        void updatePropagate(double deltaTime);

        virtual void update(double deltaTime) = 0;

        void renderPropagate();

        virtual void render() = 0;

        virtual void renderImGui() = 0;

        virtual void CustomImGui() {};

        void initializePropagate();

        virtual void initialize() = 0;

        void resize(int width, int height);

        virtual void onResize(int width, int height) = 0;

        void keyPress(int key, int scancode, int action, int mods);

        virtual void onKey(int key, int scancode, int action, int mods) = 0;

        void mouseButton(int button, int action, int mods);

        virtual void onMouseButton(int button, int action, int mods) = 0;

        void exit();

        virtual void onExit() = 0;

        void addChild(std::shared_ptr<IGameNode> child);

        void removeChild(std::shared_ptr<IGameNode> child);

        bool hasChild(std::shared_ptr<IGameNode> child);

        int childCount();

        void clearChildren();

        virtual void setParent(std::shared_ptr<IGameNode> parent) { parent_ = std::move(parent); }

        virtual std::shared_ptr<IGameNode> getParent() { return parent_.lock(); }

        virtual std::vector<std::shared_ptr<IGameNode>> getChildren() { return children_; }

        virtual bool isRoot() { return parent_.expired(); }

        virtual std::shared_ptr<IScene> getScene() = 0;

        virtual bool hasScene() = 0;

        void setScenePropagate(std::shared_ptr<IScene> scene);

        virtual void setScene(std::shared_ptr<IScene> scene) = 0;

        char *getName() { return name_; }

        void setName(char *name) { name_ = name; }

    protected:
        std::vector<std::shared_ptr<IGameNode>> children_;
    private:
        char *name_;
        std::weak_ptr<IGameNode> parent_;
        bool is_initialized_ = false;
    };

} // entre_portais

#endif //ENTREPORTAIS_IGAMENODE_HPP
