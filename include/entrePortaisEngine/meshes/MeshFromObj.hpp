//
// Created by vini84200 on 2/21/23.
//

#ifndef ENTREPORTAIS_MESHFROMOBJ_HPP
#define ENTREPORTAIS_MESHFROMOBJ_HPP

#include <functional>
#include "IMesh.hpp"
#include "entrePortaisEngine/tasks/TaskHandler.hpp"
#include "entrePortaisEngine/tasks/ITask.hpp"
#include "tiny_obj_loader.h"
#include "entrePortaisEngine/Compatibility.hpp"

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

    class ParseObjTask : public ITask {
    public:
        ParseObjTask(std::string fileName, std::function<void(tinyobj::ObjReader)>);

        TaskRunResult Run() override;

        void OnFinish() override;

        void OnCancel() override;

        std::string fileName_;
        tinyobj::ObjReader reader_;
        std::function<void(tinyobj::ObjReader)> callback_;
    };

    class MeshFromObj : public IMesh {
    public:
        MeshFromObj(std::string assetName);

        void Draw() override;

        void FinishedLoading(tinyobj::ObjReader reader);

    private:
        atomic_bool loaded_ = false;
        atomic_bool canInitialize_ = false;
        TaskHandler loadTask_;
        std::string assetName_;
        tinyobj::ObjReader reader_;

        void initializeMesh();
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
