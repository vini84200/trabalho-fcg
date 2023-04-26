#include "entrePortaisEngine/meshes/MeshFromObj.hpp"
#include "entrePortaisEngine/tasks/TaskManager.hpp"
#include "utils/matrices.h"
#include "glm/gtx/string_cast.hpp"
#include "entrePortaisEngine/meshes/ComputeNormals.hpp"
#include "entrePortaisEngine/render/TextureManager.hpp"

namespace entre_portais {

    void MeshFromObj::Draw(Shader shaderInUse, RenderPass current_pass) {
        if (loaded_) {
            UseVAO();

//            glDrawElements(GL_TRIANGLES, GetNumVertices(), GL_UNSIGNED_INT, 0);
            if (current_pass == RenderPass::FOREGROUND) {
                for (const auto &[materialId, range]: foregroundMaterialRanges_) {
                    auto [start, count] = range;
                    if (materialId < materials_.size()) {
                        auto material = materials_.at(materialId);
                        shaderInUse.setUniformVec3("material.Ka", material.ambient[0], material.ambient[1],
                                                   material.ambient[2]);
                        shaderInUse.setUniformVec3("material.Kd", material.diffuse[0], material.diffuse[1],
                                                   material.diffuse[2]);
                        shaderInUse.setUniformVec3("material.Ks", material.specular[0], material.specular[1],
                                                   material.specular[2]);
                        shaderInUse.setUniformFloat("material.q", material.shininess);
//                        shaderInUse.setUniformFloat("alpha", 1.0f); // Should be equal to 1.0f
                        unsigned int textureUsage = 0;
                        shaderInUse.setUniformInt("baseTexture", 0);
                        shaderInUse.setUniformInt("specularTexture", 1);
                        shaderInUse.setUniformInt("normalTexture", 2);
                        shaderInUse.setUniformInt("specularHighlightTexture", 3);
                        if (material.diffuse_texname != "") {
                            TextureHandle texture = TextureManager::instance().getTexture(material.diffuse_texname);
                            texture.Bind(0);
                            textureUsage |= 1;
                        }
                        if (material.specular_texname != "") {
                            TextureHandle texture = TextureManager::instance().getTexture(material.specular_texname);
                            texture.Bind(1);
                            textureUsage |= 2;
                        }
                        if (material.normal_texname != "") {
                            TextureHandle texture = TextureManager::instance().getTexture(material.normal_texname);
                            texture.Bind(2);
                            textureUsage |= 4;
                        }
                        if (material.specular_highlight_texname != "") {
                            TextureHandle texture = TextureManager::instance().getTexture(
                                    material.specular_highlight_texname);
                            texture.Bind(3);
                            textureUsage |= 8;
                        }
                        shaderInUse.setUniformUInt("material.use_texture", textureUsage);
                    } else {
//                    spdlog::warn("Material id {} out of range", materialId);
                    };
                    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *) (start * sizeof(unsigned int)));
                }
            }
            if (current_pass == RenderPass::TRANSPARENCY) {
                for (const auto &[materialId, range]: transparentMaterialRanges_) {
                    auto [start, count] = range;
                    std::optional<TextureHandle> texture;
                    if (materialId < materials_.size()) {
                        auto material = materials_.at(materialId);
                        shaderInUse.setUniformVec3("Ka", material.ambient[0], material.ambient[1], material.ambient[2]);
                        shaderInUse.setUniformVec3("KdIn", material.diffuse[0], material.diffuse[1],
                                                   material.diffuse[2]);
                        shaderInUse.setUniformVec3("Ks", material.specular[0], material.specular[1],
                                                   material.specular[2]);
                        shaderInUse.setUniformFloat("q", material.shininess);
                        shaderInUse.setUniformFloat("alpha", material.dissolve);

                        if (material.diffuse_texname != "") {
                            texture = TextureManager::instance().getTexture(material.diffuse_texname);
                            texture->Bind();
                            shaderInUse.setUniformInt("texture_", 1);
                        } else {
                            shaderInUse.setUniformInt("texture_", 0);
                        }
                    } else {
//                    spdlog::warn("Material id {} out of range", materialId);
                    };
                    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *) (start * sizeof(unsigned int)));
                    if (texture) {
                        texture->Unbind();
                    }
                }
            }
            UnbindVAO();
        } else if (canInitialize_) {
            initializeMesh(); // TODO: Encontrar um outro lugar para chamar a inicialização
        }
    }

    MeshFromObj::MeshFromObj(std::string assetName) : assetName_(assetName) {
        result_ = std::make_unique<ParsedObjResult>();
        auto tm = TaskManager::getInstance();
        tm->addTask<ParseObjTask>(assetName, [this]() {
            this->FinishedLoading();
        }, *result_.get());
    }

    void MeshFromObj::FinishedLoading() {

        canInitialize_ = true;

    }

    void MeshFromObj::initializeMesh() {
        auto vao = std::make_shared<VertexArrayBuffer>();

        BufferBuilder vboPos, vboNormal, vboTexCoord, ebo;

        std::vector<glm::vec3> &positions = result_->positions;
        std::vector<glm::vec3> &normals = result_->normals;
        std::vector<glm::vec2> &texCoords = result_->texCoords;
        std::vector<unsigned int> &indices = result_->indices;
        materials_ = result_->materials;
        std::vector<tinyobj::material_t> &materials = result_->materials;

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
        foregroundMaterialRanges_ = result_->foregroundMaterialRanges;
        spdlog::info("in {}", indices.size());
        loaded_ = true;
        result_.reset();
    }

    void MeshFromObj::StartTextureLoading(std::vector<tinyobj::material_t> materials) {
        // TODO: Usar uma task para carregar as texturas
        for (int i = 0; i < materials.size(); ++i) {
            auto &material = materials[i];
            if (!material.diffuse_texname.empty()) {
                TextureManager::instance().LoadTexture(material.diffuse_texname);
            }
            if (!material.specular_texname.empty()) {
                TextureManager::instance().LoadTexture(material.specular_texname);
            }

        }
    }

    void MeshFromObj::RenderImGui(bool *p_open) {
        ImGui::Text("Mesh From Obj");
        ImGui::Text("Asset Name: %s", assetName_.c_str());
        if (loaded_) {
            ImGui::Text("Loaded");
            ImGui::Text("Material Count: %d", materials_.size());
//            ImGui::Text("Texture Count: %d", textures_.size());
            ImGui::Text("Material Ranges: %d", foregroundMaterialRanges_.size());
            ImGui::Text("Index Count: %d", GetNumVertices());

            if (ImGui::TreeNode("Materials:")) {
                if (materials_.size() > 0) {
                    int selectedMaterial = ImGui::GetStateStorage()->GetInt(ImGui::GetID("##Material"), 0);
                    ImGui::Combo("Material", &selectedMaterial, [](void *data, int idx, const char **out_text) {
                        auto &materials = *static_cast<std::vector<tinyobj::material_t> *>(data);
                        *out_text = materials[idx].name.c_str();
                        return true;
                    }, static_cast<void *>(&materials_), materials_.size());
                    {
                        if (selectedMaterial < 0 || selectedMaterial >= materials_.size()) {
                            selectedMaterial = 0;
                        }
                        auto &material = materials_[selectedMaterial];
                        ImGui::Text("Name: %s", material.name.c_str());
                        ImGui::ColorEdit3("Ka", material.ambient);
                        ImGui::ColorEdit3("Kd", material.diffuse);
                        ImGui::ColorEdit3("Ks", material.specular);
                        ImGui::DragFloat("q", &material.shininess, 0.1f, 0.0f, 100.0f, "%.1f",
                                         ImGuiSliderFlags_Logarithmic);
                        if (!material.diffuse_texname.empty()) {
                            ImGui::Text("Texture: %s", material.diffuse_texname.c_str());
                            TextureHandle tex = TextureManager::instance().getTexture(material.diffuse_texname);
                            ImGui::Image(tex.GetImTextureID(), ImVec2(128, 128),
                                         ImVec2(0, 1), ImVec2(1, 0));
                        }
                        if (!material.specular_texname.empty()) {
                            ImGui::Text("Specular Texture: %s", material.specular_texname.c_str());
                            TextureHandle tex = TextureManager::instance().getTexture(material.specular_texname);
                            ImGui::Image(tex.GetImTextureID(), ImVec2(128, 128),
                                         ImVec2(0, 1), ImVec2(1, 0));
                        }
                    }
                    ImGui::GetStateStorage()->SetInt(ImGui::GetID("##Material"), selectedMaterial);
                } else {
                    ImGui::Text("No Materials");
                }

                ImGui::TreePop();
            }

//            if (ImGui::TreeNode("Textures:")) {
//                if (textures_.size() == 0) {
//                    ImGui::Text("No Textures");
//                }
//                for (auto &[mat, tex]: textures_) {
//                    ImGui::Text("Material: %d", mat);
//                    ImGui::Text("Texture: %s", materials_[mat].diffuse_texname.c_str());
//                    ImGui::Image(tex.GetImTextureID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
//                }
//                ImGui::TreePop();
//            }

        } else {
            ImGui::Text("Loading...");
        }
    }

    TaskRunResult ParseObjTask::Run() {
        tinyobj::ObjReader reader;
        tinyobj::ObjReaderConfig config;
        config.mtl_search_path = "./assets/materials/";

        std::string loggerName = "ParseObjTask::" + fileName_;
        Logger l = Logger(loggerName.c_str());

        l.getLogger()->info("Started Parsing {}", fileName_);
        if (!reader.ParseFromFile(fileName_, config)) {
            l.getLogger()->error("Parsing {} failed./n {}", fileName_, reader.Error());
            return TaskRunResult::FAILURE;
        }

        if (!reader.Warning().empty()) {
            spdlog::warn("Avisos ao importar obj {}: {}", fileName_, reader.Warning());
        }

        spdlog::info("Successfully parsed object {}", fileName_);
        spdlog::trace("Object has {} points in {} shapes", reader.GetAttrib().vertices.size(),
                      reader.GetShapes().size());

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        tinyobj::attrib_t original_attrib = reader.GetAttrib();
        std::vector<tinyobj::shape_t> original_shapes = reader.GetShapes();
        std::vector<tinyobj::material_t> materials = reader.GetMaterials();

        RecalculateNormals(original_attrib, original_shapes, attrib, shapes);
        CreateIndices(attrib, shapes, materials);

        return TaskRunResult::SUCCESS;
    }

    void ParseObjTask::OnFinish() {
        callback_();
    }

    void ParseObjTask::OnCancel() {
    }

    ParseObjTask::ParseObjTask(std::string fileName, std::function<void()> callback,
                               ParsedObjResult &resultPtr) :
            fileName_(fileName), callback_(callback), result_(resultPtr) {
    }

    void ParseObjTask::RecalculateNormals(tinyobj::attrib_t &original_attrib,
                                          std::vector<tinyobj::shape_t> &original_shapes,
                                          tinyobj::attrib_t &new_attrib,
                                          std::vector<tinyobj::shape_t> &new_shapes) {
        if (original_attrib.normals.size() == 0) {
            computeSmoothingShapes(original_attrib, original_shapes, new_shapes, new_attrib);
            computeAllSmoothingNormals(new_attrib, new_shapes);
        } else {
            new_attrib = original_attrib;
            new_shapes = original_shapes;
        }
    }

    void ParseObjTask::CreateIndices(tinyobj::attrib_t &attrib, std::vector<tinyobj::shape_t> &shapes,
                                     std::vector<tinyobj::material_t> materials) {
        std::unordered_map<ObjVertex, unsigned int> vertices;

        for (const auto &shape: shapes) {
            size_t num_triangles = shape.mesh.num_face_vertices.size();
            unsigned int materialId = shape.mesh.material_ids[0];
            unsigned int startIndex = result_.indices.size();

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
                        vertices[v] = static_cast<unsigned int>(result_.positions.size());
                        result_.positions.push_back(tri_positions[vertex]);
                        result_.normals.push_back(tri_normals[vertex]);
                        result_.texCoords.push_back(tri_texCoords[vertex]);
                    }

                    result_.indices.push_back(vertices[v]);
                }

            }

            if (materialId > materials.size()) {
                spdlog::warn("Material {} not found", materialId);
                // TODO: Do something for materials not found
            } else {
//                result_.materials[materialId] = materials[materialId];
            }
            result_.foregroundMaterialRanges.emplace(materialId,
                                                     std::make_pair(startIndex, result_.indices.size() - startIndex));
        }
        result_.materials = materials;
    }


} // entre_portais