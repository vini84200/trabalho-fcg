#include "entrePortaisEngine/Characters/FirstPersonCharacter.hpp"
#include "GLFW/glfw3.h"
#include "characterkinematic/PxCapsuleController.h"
#include "characterkinematic/PxController.h"
#include "entrePortaisEngine/IScene.hpp"
#include "entrePortaisEngine/Window.hpp"
#include "glm/gtx/string_cast.hpp"

namespace entre_portais {
    FirstPersonCharacter::FirstPersonCharacter(char *name) : IObject(name) {

    }

    void FirstPersonCharacter::onKey(int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_W:
                if (action == GLFW_PRESS) {
                    direction_.z = 1;
                } else if (action == GLFW_RELEASE) {
                    direction_.z = 0;
                }
                break;
            case GLFW_KEY_A:
                if (action == GLFW_PRESS) {
                    direction_.x = 1;
                } else if (action == GLFW_RELEASE) {
                    direction_.x = 0;
                }
                break;
            case GLFW_KEY_S:
                if (action == GLFW_PRESS) {
                    direction_.z = -1;
                } else if (action == GLFW_RELEASE) {
                    direction_.z = 0;
                }
                break;
            case GLFW_KEY_D:
                if (action == GLFW_PRESS) {
                    direction_.x = -1;
                } else if (action == GLFW_RELEASE) {
                    direction_.x = 0;
                }
                break;
            case GLFW_KEY_LEFT_ALT:
                if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL) {
                    // Togle cursor
                    if (action == GLFW_PRESS) {
                        spdlog::info("Togle cursor");
                        IObject::getScene()->getWindow()->showCursor(
                                !IObject::getScene()->getWindow()->isCursorVisible());
                        altMode_ = !altMode_;
                    }
                } else if (action == GLFW_PRESS) {
                    spdlog::info("Show cursor");
                    IObject::getScene()->getWindow()->showCursor(true);
                    altMode_ = true;
                } else if (action == GLFW_RELEASE) {
                    spdlog::info("Hide cursor");
                    IObject::getScene()->getWindow()->showCursor(false);
                    altMode_ = false;
                }
                break;
            case GLFW_KEY_ESCAPE:
                if (action == GLFW_PRESS) {
                    switchCameraMode();
                }
                break;
            case GLFW_KEY_LEFT_SHIFT:
                if (action == GLFW_PRESS) {
                    sprint_ = true;
                } else if (action == GLFW_RELEASE) {
                    sprint_ = false;
                }
                break;
            case GLFW_KEY_SPACE:
                if (action == GLFW_PRESS) {
                    if (isOnGround_) {
                        verticalAcceleration_ += 5.f;
                    }
                }
                break;
        }
    }

    void FirstPersonCharacter::onMouseButton(int button, int action, int mods) {

    }

    void FirstPersonCharacter::onExit() {

    }

    void FirstPersonCharacter::update(float deltaTime) {
        auto newDirection = transform_.rotateVector(direction_);
        float sprintBoost;
        if (sprint_) {
            sprintBoost = 2.0;
        } else {
            sprintBoost = 1.0;
        }

        bool hasRigidBody = rigidBody_ != nullptr;
//        if (hasRigidBody)
//        {
//            glm::vec3 velocity = rigidBody_->getVelocity();
//            glm::vec3 diffVelocity = newDirection * speed_ * sprintBoost * 10.f - velocity;
//            float diffVelocityLength = glm::length(diffVelocity);
//            float maxVelocityChange = rigidBody_->getMass() * 10.f * sprintBoost;
//            float impulse = std::min(maxVelocityChange, diffVelocityLength);
//            glm::vec3 impulseVector = impulse * glm::normalize(diffVelocity);
//            rigidBody_->applyImpulse(impulseVector);
//        }
//        else {
        //TODO: Reimplementar a movimentação usando rigidbody
//            transform_.move(newDirection * speed_ * deltaTime * sprintBoost);

//        }

//        physx::PxVec3 disp =

        physx::PxControllerState cctState;
        controller_->getState(cctState);

        isOnGround_ = cctState.collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN;
        if (!isOnGround_) {
            verticalAcceleration_ -= 9.8f * deltaTime;
        } else {
            verticalAcceleration_ = std::max(0.0f, verticalAcceleration_);
        }

        newDirection.y = verticalAcceleration_;

        controller_->move(
                physx::PxVec3(newDirection.x, newDirection.y, newDirection.z) * speed_ * sprintBoost * deltaTime, 0.0f,
                deltaTime, physx::PxControllerFilters());

        // Sync transform
        auto position = controller_->getPosition();
        transform_.setPosition(glm::vec3(position.x, position.y, position.z));


    }

    void FirstPersonCharacter::initialize() {
        loadBodyMesh();
        pauseMode_ = false;
        sprint_ = false;
        char *emptyObjectName = "empty";
        auto emptyObject = std::make_shared<EmptyObject>(emptyObjectName);
        emptyObject->getTransform()->setRotation(glm::vec3(0.0, M_PI, 0.0));
        emptyObject1_ = emptyObject;
        addChild(emptyObject);
        char *emptyObjectName2 = "empty2";
        auto emptyObject2 = std::make_shared<EmptyObject>(emptyObjectName2);
        emptyObject2_ = emptyObject2;
        emptyObject1_->addChild(emptyObject2);
        char *cameraName = "camera";
        auto camera = std::make_shared<Camera>(cameraName, M_PI_2, 0.1, 10000.0, 1.8);
        camera_ = camera;
        emptyObject2_->addChild(camera);
        auto renderer = IObject::getScene()->getRenderer();
        loadShader("phong");
        submit(renderer, RenderPass::FOREGROUND);
//        std::unique_ptr<ICollider> cubeCollider = std::make_unique<BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f),
//                                                                                modelMatrix_);
//        rigidBody_ = std::make_unique<RigidBody>(&modelMatrix_, std::move(cubeCollider),
//                                                 *this->getScene()->getPhysicsEngine().get(),
//                                                 this->transform_);
        physx::PxCapsuleControllerDesc desc;
        desc.height = 1.8f;
        desc.radius = 0.3f;
        desc.material = getScene()->getPhysicsEngine()->getPhysics().createMaterial(0.5f, 0.5f, 0.6f);
        desc.position = physx::PxExtendedVec3(transform_.getPosition().x, transform_.getPosition().y,
                                              transform_.getPosition().z);
        desc.slopeLimit = 0.0f;
        desc.contactOffset = 0.1f;
        desc.stepOffset = 0.1f;
        desc.invisibleWallHeight = 0.0f;
        desc.maxJumpHeight = 0.0f;
        desc.reportCallback = nullptr;
        controller_ = getScene()->getPhysicsEngine()->getControllerManager().createController(desc);
        controller_->setUserData(this);
    }

    std::shared_ptr<Camera> FirstPersonCharacter::getCamera() {
        return camera_;
    }

    void FirstPersonCharacter::onResize(int width, int height) {

    }

    void FirstPersonCharacter::onMouseDeltaMovement(glm::vec2 delta) {
        if (altMode_) {
            return;
        }
        if (!pauseMode_) {
            glm::vec2 d = 0.001f * delta;
            glm::vec3 rightCamera = glm::cross(glm::vec3(0.0, 1.0, 0.0), camera_->getTransform()->getForward());
            glm::quat rotationHead = glm::quat(glm::vec3(-d.y, 0, 0));
            glm::quat rotationBody = glm::quat(glm::vec3(0, -d.x, 0));
            camera_->getTransform()->rotateBy(rotationHead);
            glm::vec3 newRightCamera = glm::cross(glm::vec3(0.0, 1.0, 0.0), camera_->getTransform()->getForward());
            if (glm::dot(rightCamera, newRightCamera) < 0) {
                camera_->getTransform()->rotateBy(conjugate(rotationHead));
            }
            getTransform()->rotateBy(rotationBody);
        } else {
            glm::vec2 d = 0.001f * delta;
            glm::vec3 rightEmptyObject = glm::cross(glm::vec3(0.0, 1.0, 0.0),
                                                    emptyObject2_->getTransform()->getForward());
            glm::quat rotationHead = glm::quat(glm::vec3(0, 0, d.y));
            glm::quat rotationBody = glm::quat(glm::vec3(0, -d.x, 0));
            emptyObject2_->getTransform()->rotateBy(rotationHead);
            glm::vec3 newRightEmptyObject = glm::cross(glm::vec3(0.0, 1.0, 0.0),
                                                       emptyObject2_->getTransform()->getForward());
            if (glm::dot(rightEmptyObject, newRightEmptyObject) < 0) {
                emptyObject2_->getTransform()->rotateBy(conjugate(rotationHead));
            }
            getTransform()->rotateBy(rotationBody);
        }
    }

    void FirstPersonCharacter::switchCameraMode() {
        if (pauseMode_) { // Pause ON -> OFF
            auto newCameraPosition = glm::vec3(0.0, 0.0, 0.0);
            getCamera()->getTransform()->setPosition(newCameraPosition);
            getCamera()->getTransform()->setRotation(emptyObject2_->getTransform()->getRotation());
//            emptyObject1_->getTransform()->setRotation(glm::vec3(0.0, 0.0, 0.0));
            emptyObject2_->getTransform()->setRotation(glm::vec3(0.0, 0.0, 0.0));
        } else { // Pause OFF -> ON
            auto cameraEuler = getCamera()->getTransform()->getRotationEulerZYX();
            auto cameraPosition = getCamera()->getTransform()->getPosition();
            auto newCameraPosition = glm::vec3(cameraPosition.x,
                                               cameraPosition.y,
                                               cameraPosition.z + M_PI_2);
            getCamera()->getTransform()->setPosition(newCameraPosition);
            emptyObject2_->getTransform()->setRotation(getCamera()->getTransform()->getRotation());
            getCamera()->getTransform()->setRotation(glm::vec3(0.0, 0.0, 0.0));
        }
        pauseMode_ = !pauseMode_;
    }

    void FirstPersonCharacter::teleport(glm::vec3 position) {
        controller_->setPosition(physx::PxExtendedVec3(position.x, position.y, position.z));

    }
} // entre_portais