//
// Created by vini84200 on 3/19/23.
//

#ifndef ENTREPORTAIS_AMBIENTFROMTEXTURE_HPP
#define ENTREPORTAIS_AMBIENTFROMTEXTURE_HPP

#include "entrePortaisEngine/IObject.hpp"
#include "entrePortaisEngine/render/Texture.hpp"

namespace entre_portais {

    class AmbientMesh : public IMesh {
    public:
        AmbientMesh(std::array<std::string, 6> faceTextures);

        void Draw(Shader shaderInUse, RenderPass current_pass) override;

        int textureid_;
    };

    class AmbientFromTexture : public IObject {
    public:
        AmbientFromTexture(std::array<std::string, 6> texturePath);

        void onKey(int key, int scancode, int action, int mods) override;

        void onMouseButton(int button, int action, int mods) override;

        void onExit() override;


        void update(float deltaTime) override;

        void initialize() override;

        void onResize(int width, int height) override;

    private:
        std::array<std::string, 6> texturePath_;
    };


} // entre_portais

#endif //ENTREPORTAIS_AMBIENTFROMTEXTURE_HPP
