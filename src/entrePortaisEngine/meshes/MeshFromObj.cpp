#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"

namespace entre_portais {

    void MeshFromObj::Draw() {
        if (loaded_) {
            UseVAO();

            glDrawElements(GL_TRIANGLES, GetNumVertices(), GL_UNSIGNED_INT, 0);
            UnbindVAO();
        } else if (canInitialize_) {
            initializeMesh();
        }
    }

    MeshFromObj::MeshFromObj(std::string assetName) : assetName_(assetName) {
        auto tm = TaskManager::getInstance();
        tm->addTask<ParseObjTask>(assetName, [this](tinyobj::ObjReader reader) {
            this->FinishedLoading(std::move(reader));
        });
    }

    void MeshFromObj::FinishedLoading(tinyobj::ObjReader reader) {
        auto inattrib = reader.GetAttrib();
        auto materials = reader.GetMaterials();
        auto inshapes = reader.GetShapes();

        reader_ = std::move(reader);
        canInitialize_ = true;


    }

    void MeshFromObj::initializeMesh() {
        spdlog::info("Points");

        auto attrib = reader_.GetAttrib();
        auto shapes = reader_.GetShapes();
        auto materials = reader_.GetMaterials();

        auto vao = std::make_shared<VertexArrayBuffer>();

        BufferBuilder vboPos, vboColor, ebo;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> colours;
        std::vector<unsigned int> indices;

        assert(shapes.size() == 1); // Apenas uma Shape

        for (size_t shape = 0; shape < shapes.size(); ++shape) {
            size_t num_triangles = shapes[shape].mesh.num_face_vertices.size();

            for (size_t triangle = 0; triangle < num_triangles; ++triangle) {
                assert(shapes[shape].mesh.num_face_vertices[triangle] == 3);

                for (size_t vertex = 0; vertex < 3; ++vertex) {
                    tinyobj::index_t idx = shapes[shape].mesh.indices[3 * triangle + vertex];

                    indices.push_back(3 * triangle + vertex);

                    const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                    const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                    const float vz = attrib.vertices[3 * idx.vertex_index + 2];
                    positions.push_back({vx, vy, vz});
                    colours.push_back({0.2, 0.5, 0.7});
//                    spdlog::info("TRI {} vert {} = ({}, {}, {})", (int) triangle, (int)vertex, vx, vy, vz);

                    // Inspecionando o código da tinyobjloader, o aluno Bernardo
                    // Sulzbach (2017/1) apontou que a maneira correta de testar se
                    // existem normais e coordenadas de textura no ObjModel é
                    // comparando se o índice retornado é -1. Fazemos isso abaixo.

                    if (idx.normal_index != -1) {
                        const float nx = attrib.normals[3 * idx.normal_index + 0];
                        const float ny = attrib.normals[3 * idx.normal_index + 1];
                        const float nz = attrib.normals[3 * idx.normal_index + 2];
//                        normal_coefficients.push_back( nx ); // X
//                        normal_coefficients.push_back( ny ); // Y
//                        normal_coefficients.push_back( nz ); // Z
//                        normal_coefficients.push_back( 0.0f ); // W
                    }

                    if (idx.texcoord_index != -1) {
                        const float u = attrib.texcoords[2 * idx.texcoord_index + 0];
                        const float v = attrib.texcoords[2 * idx.texcoord_index + 1];
//                        texture_coefficients.push_back( u );
//                        texture_coefficients.push_back( v );
                    }
                }
            }
        }

        vboPos.addAttribute(POSITIONS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboPos.setData(positions.data(), positions.size() * sizeof(glm::vec3));
        vboPos.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboPos);

        vboColor.addAttribute(COLORS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboColor.setData(colours.data(), colours.size() * sizeof(glm::vec3));
        vboColor.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboColor);

        ebo.setData(indices.data(), indices.size() * sizeof(unsigned int));
        ebo.setUsage(GL_STATIC_DRAW);
        vao->addEBOToQueue(&ebo);

        vao->Commit();
        SetVAO(std::move(vao), indices.size());
        spdlog::info("in {}", indices.size());
        loaded_ = true;
    }

    TaskRunResult ParseObjTask::Run() {
        tinyobj::ObjReaderConfig config;
        config.mtl_search_path = "./assets/materials/"; //TODO: Setar a posição correta dos materiais

        spdlog::info("Started Parsing {}", fileName_);
        if (!reader_.ParseFromFile(fileName_, config)) {
            if (!reader_.Error().empty()) {
                spdlog::error("Erros ao importar obj {}: {}", fileName_, reader_.Error());
            }
            return TaskRunResult::FAILURE;
        }

        if (!reader_.Warning().empty()) {
            spdlog::warn("Avisos ao importar obj {}: {}", fileName_, reader_.Warning());
        }

        spdlog::info("Finished Parsing {} Successfuly", fileName_);

        return TaskRunResult::SUCCESS;
    }

    void ParseObjTask::OnFinish() {
        callback_(std::move(reader_));
    }

    void ParseObjTask::OnCancel() {
    }

    ParseObjTask::ParseObjTask(std::string fileName, std::function<void(tinyobj::ObjReader)> callback) :
            fileName_(fileName), callback_(callback) {
    }
} // entre_portais