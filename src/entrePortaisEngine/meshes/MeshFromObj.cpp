#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "utils/matrices.h"
#include "glm/gtx/string_cast.hpp"
#include "entrePortaisEngine/meshes/ComputeNormals.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"

namespace entre_portais {

    void MeshFromObj::Draw(Shader shaderInUse) {
        if (loaded_) {
            UseVAO();

//            glDrawElements(GL_TRIANGLES, GetNumVertices(), GL_UNSIGNED_INT, 0);

            for (auto [materialId, range]: materialRanges_) {
                auto [start, count] = range;
                std::optional<Texture> texture;
                if (materialId < reader_.GetMaterials().size()) {
                    auto material = reader_.GetMaterials().at(materialId);


                    shaderInUse.setUniformVec3("Ka", material.ambient[0], material.ambient[1], material.ambient[2]);
                    shaderInUse.setUniformVec3("KdIn", material.diffuse[0], material.diffuse[1], material.diffuse[2]);
                    shaderInUse.setUniformVec3("Ks", material.specular[0], material.specular[1], material.specular[2]);
                    shaderInUse.setUniformFloat("q", material.shininess);
                    if (material.diffuse_texname != "") {
                        texture = TextureManager::instance().getTexture(material.diffuse_texname);
                        texture->Bind();
                        shaderInUse.setUniformInt("texture_", 1);
                    } else {
                        shaderInUse.setUniformInt("texture_", 0);
                    }
                };
                glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *) (start * sizeof(unsigned int)));
                if (texture) {
                    texture->Unbind();
                }
            }
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
        auto preattrib = reader_.GetAttrib();
        auto preshapes = reader_.GetShapes();
        auto materials = reader_.GetMaterials();

        auto vao = std::make_shared<VertexArrayBuffer>();

        BufferBuilder vboPos, vboNormal, vboTexCoord, ebo;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<unsigned int> indices;

        std::unordered_map<ObjVertex, unsigned int> vertices;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;

        if (materials.size() > 0) {
            spdlog::info("Materials:");
        } else {
            spdlog::info("No materials");
        }
        for (auto &m: materials) {
            spdlog::info("Material: {} Ka: {} Kd: {} Ks: {} q: {} Tex: {}",
                         m.name, glm::to_string(glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2])),
                         glm::to_string(glm::vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2])),
                         glm::to_string(glm::vec3(m.specular[0], m.specular[1], m.specular[2])),
                         m.shininess, m.diffuse_texname);

        }


        if (attrib.normals.size() == 0) {
            computeSmoothingShapes(preattrib, preshapes, shapes, attrib);
            computeAllSmoothingNormals(attrib, shapes);
        }


        for (const auto &shape: shapes) {
            size_t num_triangles = shape.mesh.num_face_vertices.size();
            unsigned int materialId = shape.mesh.material_ids[0];
            unsigned int startIndex = indices.size();

            for (size_t triangle = 0; triangle < num_triangles; ++triangle) {
                assert(shape.mesh.num_face_vertices[triangle] == 3);

                glm::vec3 tri_positions[3];
                glm::vec3 tri_normals[3];
                glm::vec3 tri_colors[3];
                glm::vec2 tri_texCoords[3];


                for (size_t vertex = 0; vertex < 3; ++vertex) {
                    tinyobj::index_t idx = shape.mesh.indices[3 * triangle + vertex];

                    const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                    const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                    const float vz = attrib.vertices[3 * idx.vertex_index + 2];
                    tri_positions[vertex] = {vx, vy, vz};

                    if (idx.normal_index != -1) {
                        const float nx = attrib.normals[3 * idx.normal_index + 0];
                        const float ny = attrib.normals[3 * idx.normal_index + 1];
                        const float nz = attrib.normals[3 * idx.normal_index + 2];
                        tri_normals[vertex] = {nx, ny, nz};
                    }

                    if (idx.texcoord_index != -1) {
                        const float u = attrib.texcoords[2 * idx.texcoord_index + 0];
                        const float v = attrib.texcoords[2 * idx.texcoord_index + 1];
                        tri_texCoords[vertex] = {u, v};
                    }
                    tri_colors[vertex] = {1, 1, 1};

                    ObjVertex v(tri_positions[vertex], tri_texCoords[vertex], tri_normals[vertex], tri_colors[vertex]);

                    if (vertices.count(v) == 0) {
                        vertices[v] = static_cast<unsigned int>(positions.size());
                        positions.push_back(tri_positions[vertex]);
                        normals.push_back(tri_normals[vertex]);
                        texCoords.push_back(tri_texCoords[vertex]);
                        //TODO: Adicionar os outros buffers
                    }

                    indices.push_back(vertices[v]);
                }

            }

            materialRanges_.emplace(materialId, std::make_pair(startIndex, indices.size() - startIndex));
            if (materialId > materials.size()) {
//                spdlog::warn("Material {} not found", materialId);
            }
        }

        StartTextureLoading(materials);

        vboPos.addAttribute(POSITIONS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboPos.setData(positions.data(), positions.size() * sizeof(glm::vec3));
        vboPos.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboPos);

        vboNormal.addAttribute(NORMALS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        vboNormal.setData(normals.data(), normals.size() * sizeof(glm::vec3));
        vboNormal.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboNormal);

        vboTexCoord.addAttribute(TEXTURE_COORDINATES_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
        vboTexCoord.setData(texCoords.data(), texCoords.size() * sizeof(glm::vec2));
        vboTexCoord.setUsage(GL_STATIC_DRAW);
        vao->addBufferToQueue(&vboTexCoord);

        ebo.setData(indices.data(), indices.size() * sizeof(unsigned int));
        ebo.setUsage(GL_STATIC_DRAW);
        vao->addEBOToQueue(&ebo);

        vao->Commit();
        SetVAO(std::move(vao), indices.size());
        spdlog::info("in {}", indices.size());
        loaded_ = true;
    }

    void MeshFromObj::StartTextureLoading(std::vector<tinyobj::material_t> materials) {
        // TODO: Usar uma task para carregar as texturas
        for (int i = 0; i < materials.size(); ++i) {
            auto &material = materials[i];
            if (material.diffuse_texname.empty()) {
                continue;
            }
            textures_.insert({i, TextureManager::instance().getTexture(material.diffuse_texname)});
        }
    }

    TaskRunResult ParseObjTask::Run() {
        tinyobj::ObjReaderConfig config;
        config.mtl_search_path = "./assets/materials/"; //TODO: Setar a posição correta dos materiais

        std::string loggerName = "ParseObjTask::" + fileName_;
        Logger l = Logger(loggerName.c_str());

        l.getLogger()->info("Started Parsing {}", fileName_);
        if (!reader_.ParseFromFile(fileName_, config)) {
            l.getLogger()->error("Parsing {} failed./n {}", fileName_, reader_.Error());
            return TaskRunResult::FAILURE;
        }

        if (!reader_.Warning().empty()) {
            spdlog::warn("Avisos ao importar obj {}: {}", fileName_, reader_.Warning());
        }

        spdlog::info("Successfully parsed object {}", fileName_);
        spdlog::trace("Object has {} points in {} shapes", reader_.GetAttrib().vertices.size(),
                      reader_.GetShapes().size());

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