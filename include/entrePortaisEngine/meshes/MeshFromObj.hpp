#ifndef ENTREPORTAIS_MESHFROMOBJ_HPP
#define ENTREPORTAIS_MESHFROMOBJ_HPP

#include <functional>
#include "IMesh.hpp"
#include "entrePortaisEngine/tasks/TaskHandler.hpp"
#include "entrePortaisEngine/tasks/ITask.hpp"
#include "tiny_obj_loader.h"
#include "entrePortaisEngine/Compatibility.hpp"
#include "entrePortaisEngine/render/Texture.hpp"
#include "spdlog/spdlog.h"
#include "entrePortaisEngine/render/Renderer.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>


namespace entre_portais {
    struct ObjVertex {
        glm::vec3 pos;
        glm::vec2 texCoord;
        glm::vec3 normals;
        glm::vec3 color;

        ObjVertex(glm::vec3 pos, glm::vec2 texCoord, glm::vec3 normals, glm::vec3 color) :
                pos(pos), texCoord(texCoord), normals(normals), color(color) {};

        bool operator==(const ObjVertex &other) const {
            return pos == other.pos && texCoord == other.texCoord && normals == other.normals && color == other.color;
        }
    };

    struct ParsedObjResult {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<unsigned int> indices;
        std::vector<tinyobj::material_t> materials;
        std::unordered_multimap<unsigned int, std::pair<unsigned int, unsigned int>> foregroundMaterialRanges;
        std::unordered_multimap<unsigned int, std::pair<unsigned int, unsigned int>> transparentMaterialRanges;


        ~ParsedObjResult() {
            spdlog::info("Deleting ParsedObjResult");
        }
    };

    class ParseObjTask : public ITask {
    public:
        ParseObjTask(std::string fileName, std::function<void()>, ParsedObjResult &resultPtr);

        TaskRunResult Run() override;

        void OnFinish() override;

        void OnCancel() override;

        std::string fileName_;
        ParsedObjResult &result_;
        std::function<void()> callback_;

        void RecalculateNormals(tinyobj::attrib_t &original_attrib, std::vector<tinyobj::shape_t> &original_shapes,
                                tinyobj::attrib_t &new_attrib, std::vector<tinyobj::shape_t> &new_shapes);

        void CreateIndices(tinyobj::attrib_t &attrib, std::vector<tinyobj::shape_t> &shapes,
                           std::vector<tinyobj::material_t> materials);
    };

    class MeshFromObj : public IMesh {
    public:
        MeshFromObj(std::string assetName);

        void Draw(Shader shaderInUse, RenderPass current_pass) override;

        void FinishedLoading();

        void RenderImGui(bool *p_open) override;

    private:
        atomic_bool loaded_ = false;
        atomic_bool canInitialize_ = false;
        std::string assetName_;
        std::unique_ptr<ParsedObjResult> result_;


        std::vector<tinyobj::material_t> materials_;
        std::unordered_multimap<unsigned int, std::pair<unsigned int, unsigned int>> foregroundMaterialRanges_;
        std::unordered_multimap<unsigned int, std::pair<unsigned int, unsigned int>> transparentMaterialRanges_;

        void initializeMesh();

        void StartTextureLoading(std::vector<tinyobj::material_t> materials);
    };

} // entre_portais

namespace std {
    template<>
    struct hash<entre_portais::ObjVertex> {
        size_t operator()(entre_portais::ObjVertex const &vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                     (hash<glm::vec2>()(vertex.texCoord) << 1)) >> 1) ^
                   (hash<glm::vec3>()(vertex.normals) << 1) ^
                   (hash<glm::vec3>()(vertex.color) << 1);
        }
    };
}

#endif //ENTREPORTAIS_MESHFROMOBJ_HPP
