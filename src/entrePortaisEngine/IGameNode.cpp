#include "entrePortaisEngine/IGameNode.hpp"

#include <algorithm>
#include <stdexcept>

namespace entre_portais {
    void IGameNode::updatePropagate(float deltaTime) {
        update(deltaTime);
        for (auto &child: children_) {
            child->updatePropagate(deltaTime);
        }
    }

    void IGameNode::preRender() {
        calculateModelMatrix();
    }

    void IGameNode::preRenderPropagate() {
        preRender();
        for (auto &child: children_) {
            child->preRenderPropagate();
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
        auto s = std::enable_shared_from_this<IGameNode>::shared_from_this();
        if (s == child) {
            throw std::runtime_error("Cannot add self as child");
        }

        if (s == nullptr) {
            throw std::runtime_error("Cannot add child to null parent");
        }


        if (child->getParent() != s) {
            if (child->getParent() != nullptr) {
                child->getParent()->removeChild(child);
            }
            child->setParent(s);
            if (hasScene()) {
                child->setScene(getScene());
            }
            children_.push_back(child);
        }
        if (is_initialized_) {
            child->initializePropagate();
        }
    }

    void IGameNode::removeChild(std::shared_ptr<IGameNode> child) {
        children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
        child->setParent(nullptr);
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
        if (!is_initialized_) {
            initialize();
            is_initialized_ = true;
        }
        for (auto &child: children_) {
            child->initializePropagate();
        }
    }

    void IGameNode::setScenePropagate(std::shared_ptr<IScene> scene) {
        setScene(scene);
        for (auto &child: children_) {
            child->setScenePropagate(scene);
        }
    }

    glm::mat4 *entre_portais::IGameNode::getParentModelMatrix() {
        auto parent_obj = getParent().get();
        // Vai ser um nullptr se o parent não for do tipo IObject
        if (parent_obj) {
            return &parent_obj->modelMatrix_;
        }
        return nullptr;
    }

    std::shared_ptr<IGameNode> IGameNode::sharedPtrFromIGameNode() {
        return shared_from_this();
    }

    void IGameNode::calculateModelMatrix() {
        auto pm = getParentModelMatrix();
        if (pm == nullptr) {
            modelMatrix_ = transform_.getModelMatrix();
        } else {
            modelMatrix_ = (*pm) * transform_.getModelMatrix();
        }
    }
}  // namespace entre_portais