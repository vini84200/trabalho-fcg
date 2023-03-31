#ifndef ENTREPORTAIS_GUIOBJECT_HPP
#define ENTREPORTAIS_GUIOBJECT_HPP

#include <variant>
#include "entrePortaisEngine/render/IRenderable.hpp"
#include "entrePortaisEngine/IObject.hpp"

namespace entre_portais {

    using zIndex_t = float;

    enum Anchor {
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        CENTER
    };

    /*
     * Scale Constraints
     */
    struct FixedScale {
        float size;

        FixedScale(float size) : size(size) {}
    };

    struct RelativeScale {
        float size_ratio;

        RelativeScale(float size_ratio) : size_ratio(size_ratio) {}
    };

    // Use the visit to get the current size
    struct ScaleXConstraintsVisit {
        float window_width;
        float window_height;

        ScaleXConstraintsVisit(float window_width, float window_height) : window_width(window_width),
                                                                          window_height(window_height) {}

        float operator()(FixedScale scale) const {
            return scale.size;
        }

        float operator()(RelativeScale scale) const {
            return window_width * scale.size_ratio;
        }
    };

    struct ScaleYConstraintsVisit {
        float window_width;
        float window_height;

        ScaleYConstraintsVisit(float window_width, float window_height) : window_width(window_width),
                                                                          window_height(window_height) {}

        float operator()(FixedScale scale) const {
            return scale.size;
        }

        float operator()(RelativeScale scale) const {
            return window_height * scale.size_ratio;
        }
    };

    using ScaleConstraint = std::variant<FixedScale, RelativeScale>;

    /*
     * Position Constraints
     */
    struct FixedPosition {
        float position;

        FixedPosition(float position) : position(position) {}
    };

    struct FixedToAnchorPosition {
        Anchor anchor;
        float offset;

        FixedToAnchorPosition(float offset, Anchor anchor) : offset(offset), anchor(anchor) {}
    };

    struct RelativePosition {
        float position_ratio;
        Anchor anchor;

        RelativePosition(float position_ratio, Anchor anchor) : position_ratio(position_ratio), anchor(anchor) {}
    };

    struct RelativeToParentPosition {
        float position_ratio;
        Anchor anchor;

        RelativeToParentPosition(float position_ratio, Anchor anchor) : position_ratio(position_ratio),
                                                                        anchor(anchor) {}
    };

    struct PositionXConstraintsVisit {
        PositionXConstraintsVisit(float window_width, float window_height) : window_width(window_width),
                                                                             window_height(window_height) {}

        float operator()(FixedPosition position) const {
            return position.position;
        }

        float operator()(RelativePosition position) const {
            switch (position.anchor) {
                case LEFT:
                case TOP:
                    return window_width * position.position_ratio;
                    break;
                case RIGHT:
                case BOTTOM:
                    return window_width - window_width * position.position_ratio;
                    break;
                case CENTER:
                    return window_width / 2 - window_width * position.position_ratio;
                    break;
            }
        }

        float operator()(RelativeToParentPosition position) const {
            // TODO: Implement this correctly
            spdlog::warn("RelativeToParentPosition not implemented yet");
            return 0;
        }

        float operator()(FixedToAnchorPosition position) const {
            switch (position.anchor) {
                case LEFT:
                    return window_width - position.offset;
                    break;
                case RIGHT:
                    return position.offset;
                    break;
                case TOP:
                    break;
                case BOTTOM:
                    break;
                case CENTER:
                    return window_width / 2 - position.offset;
                    break;
            }
            return 0;
        }

    private:
        float window_width;
        float window_height;
    };

    struct PositionYConstraintsVisit {
        PositionYConstraintsVisit(float window_width, float window_height) : window_width(window_width),
                                                                             window_height(window_height) {}

        float operator()(FixedPosition position) const {
            return position.position;
        }

        float operator()(RelativePosition position) const {
            switch (position.anchor) {
                case LEFT:
                case TOP:
                    return window_height * position.position_ratio;
                    break;
                case RIGHT:
                case BOTTOM:
                    return window_height - window_height * position.position_ratio;
                    break;
                case CENTER:
                    return window_height / 2 - window_height * position.position_ratio;
                    break;
            }
        }

        float operator()(RelativeToParentPosition position) const {
            // TODO: Implement this correctly
            spdlog::warn("RelativeToParentPosition not implemented yet");
            return 0;
        }

        float operator()(FixedToAnchorPosition position) const {
            switch (position.anchor) {
                case LEFT:
                    break;
                case RIGHT:
                    break;
                case TOP:
                    return window_height - position.offset;
                    break;
                case BOTTOM:
                    return position.offset;
                    break;
                case CENTER:
                    return window_height / 2 - position.offset;
                    break;
            }
            return 0;
        }

    private:
        float window_width;
        float window_height;
    };

    using PositionConstraint = std::variant<FixedPosition, RelativePosition, RelativeToParentPosition, FixedToAnchorPosition>;

    /*
     * GuiObject
     */


    class GuiObject : public IObject {
    public:
        GuiObject(const char *name);

        GuiObject(const char *name, PositionConstraint x, PositionConstraint y, ScaleConstraint width,
                  ScaleConstraint height, zIndex_t zIndex);

        void render(RenderPass current_pass) override;

        void onResize(int width, int height) override;

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;

        /*
         * Events of GuiObject
         */

        void onMouseEnter();

        void onMouseLeave();

        // Pos is in pixels from the center of the object's bounding box
        virtual void onClick(float posX, float posY) {};


        /*
         * Getters and setters
         */

        inline glm::vec2 getPosToNDC() const {
            return glm::vec2(-currentPosX_ / currentWindowWidth_ * 2 + 1, currentPosY_ / currentWindowHeight_ * 2 - 1);
        }

        inline glm::vec2 getSizeToNDC() const {
            return glm::vec2(currentWidth_ / currentWindowWidth_, currentHeight_ / currentWindowHeight_);
        }

        inline glm::vec2 getBBMinScreenSpace() const {
            return glm::vec2(currentWindowWidth_ - currentPosX_ - currentWidth_ / 2,
                             currentWindowHeight_ - currentPosY_ - currentHeight_ / 2);
        }

        inline glm::vec2 getBBMaxScreenSpace() const {
            return glm::vec2(currentWindowWidth_ - currentPosX_ + currentWidth_ / 2,
                             currentWindowHeight_ - currentPosY_ + currentHeight_ / 2);
        }

        inline bool isInsideBB(glm::vec2 point) const {
            return isInsideBB(point.x, point.y);
        }

        inline bool isInsideBB(float x, float y) const {
            return x >= getBBMinScreenSpace().x && x <= getBBMaxScreenSpace().x &&
                   y >= getBBMinScreenSpace().y && y <= getBBMaxScreenSpace().y;
        }

        bool isInHoverState() const;


        const PositionConstraint &getXConstraint() const;

        void setX(const PositionConstraint &x);

        const PositionConstraint &getYConstraint() const;

        void setY(const PositionConstraint &y);

        void setWitdhtConstraint(const ScaleConstraint &width);

        void setHeightConstraint(const ScaleConstraint &height);

        float getPositionX() const;

        float getPositionY() const;

        float getWidth() const;

        float getHeight() const;

        zIndex_t getZIndex() const;

        void setZIndex(zIndex_t zIndex);

    protected:
        void setupUniformsForGui(Shader shader);

        void registerGuiObject(std::shared_ptr<Renderer> renderer, std::string shader_name);

        void drawQuad(VertexArrayBuffer *quadVao);


    private:
        PositionConstraint x_;
        PositionConstraint y_;
        ScaleConstraint width_;
        ScaleConstraint height_;

        float currentPosX_;
        float currentPosY_;
        float currentWidth_;
        float currentHeight_;

        float currentWindowWidth_ = 600;
        float currentWindowHeight_ = 800;

        zIndex_t zIndex_;

        // Private methods
        void recalculatePosition();
    };

} // entre_portais

#endif //ENTREPORTAIS_GUIOBJECT_HPP
