#ifndef ENTREPORTAIS_EASYMESH_HPP
#define ENTREPORTAIS_EASYMESH_HPP

#include "entrePortaisEngine/meshes/IMesh.hpp"
#include "entrePortaisEngine/render/ShadersManager.hpp"

namespace entre_portais {
    struct EasyVertex {
        float x, y, z;
        float r, g, b;
    };

    struct ManyVertices {
        std::vector<EasyVertex> vertices;
        std::vector<unsigned int> indices;
    };


    class EasyMesh : public IMesh {
    public:
        EasyMesh(ManyVertices vertices, std::string shaderName);

        ~EasyMesh() override {};

        EasyMesh(const EasyMesh &other) = delete;

        EasyMesh &operator=(const EasyMesh &other) = delete;

        EasyMesh(EasyMesh &&other) = delete;

        EasyMesh &operator=(EasyMesh &&other) = delete;

        void Draw(Shader shaderInUse) override;
    };

} // entre_portais

#endif //ENTREPORTAIS_EASYMESH_HPP
