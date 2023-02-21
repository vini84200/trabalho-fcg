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

namespace entre_portais {

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
        _Atomic bool loaded_ = false;
        _Atomic bool canInitialize_ = false;
        TaskHandler loadTask_;
        std::string assetName_;
        tinyobj::ObjReader reader_;

        void initializeMesh();
    };

} // entre_portais

#endif //ENTREPORTAIS_MESHFROMOBJ_HPP
