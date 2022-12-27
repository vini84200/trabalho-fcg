#ifndef ENTREPORTAIS_IGAMENODE_HPP
#define ENTREPORTAIS_IGAMENODE_HPP

#include <memory>
#include <utility>
#include <vector>

namespace entre_portais {

    class IGameNode {
        /* Objeto que participa da hierarquia na renderização e
         * propagação de eventos.*/

    public:
        IGameNode() = default;

        virtual ~IGameNode() = default;

        void updatePropagate();

        virtual void update() = 0;

        void renderPropagate();

        virtual void render() = 0;

        virtual void preRender() {};

        virtual void postRender() {};

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

        virtual std::shared_ptr<IGameNode> getParent() { return parent_; }

        virtual bool isRoot() { return parent_ == nullptr; }

    private:
        std::shared_ptr<IGameNode> parent_;
        std::vector<std::shared_ptr<IGameNode>> children_;
    };

} // entre_portais

#endif //ENTREPORTAIS_IGAMENODE_HPP
