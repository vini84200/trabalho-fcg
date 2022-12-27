#include <algorithm>
#include "entrePortaisEngine/IGameNode.hpp"

namespace entre_portais {
    void IGameNode::updatePropagate() {
        update();
        for (auto &child: children_) {
            child->updatePropagate();
        }
    }

    void IGameNode::renderPropagate() {
        render();
        for (auto &child: children_) {
            child->renderPropagate();
        }
    }

    void IGameNode::resize(int width, int height) {
        onResize(width, height);
        for (auto &child: children_) {
            child->resize(width, height);
        }
    }

    void IGameNode::keyPress(int key, int scancode, int action, int mods) {
        onKey(key, scancode, action, mods);
        for (auto &child: children_) {
            child->keyPress(key, scancode, action, mods);
        }
    }

    void IGameNode::mouseButton(int button, int action, int mods) {
        onMouseButton(button, action, mods);
        for (auto &child: children_) {
            child->mouseButton(button, action, mods);
        }
    }

    void IGameNode::exit() {
        onExit();
        for (auto &child: children_) {
            child->exit();
        }
    }

    void IGameNode::addChild(std::shared_ptr<IGameNode> child) {
        child->initializePropagate();
        children_.push_back(child);
    }

    void IGameNode::removeChild(std::shared_ptr<IGameNode> child) {
        children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
    }

    bool IGameNode::hasChild(std::shared_ptr<IGameNode> child) {
        return std::find(children_.begin(), children_.end(), child) != children_.end();
    }

    int IGameNode::childCount() {
        return children_.size();
    }

    void IGameNode::clearChildren() {
        children_.clear();
    }

    void IGameNode::initializePropagate() {
        initialize();
        for (auto &child: children_) {
            child->initializePropagate();
        }
    }
} // entre_portais