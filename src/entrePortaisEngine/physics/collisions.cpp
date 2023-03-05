#include "entrePortaisEngine/physics/collisions.hpp"
#include "spdlog/spdlog.h"
#include "glm/gtx/string_cast.hpp"
#include <vector>

#define GTK_EPA_MAX_ITER 32
namespace entre_portais::collisions {

    namespace GJK_EPA {
        // FONTE: https://blog.winter.dev/2020/gjk-algorithm/
        // FONTE: https://blog.winter.dev/2020/epa-algorithm/
        // ESSA PARTE DO CÓDIGO FOI ADAPTADA DO CÓDIGO DO AUTOR ACIMA CITADO
        glm::vec3 GJKCollider::findSupportPoint(const GJKCollider &other, const glm::vec3 direction) const {
            return this->findFurtherPoint(direction) - other.findFurtherPoint(-direction);
        }

        glm::vec3 CubeGJKCollider::findFurtherPoint(glm::vec3 direction) const {
            glm::vec3 result = glm::vec3(0.0f, 0.0f, 0.0f);
            float max = FLT_MIN;
            for (int i = 0; i < 8; i++) {
                float dot = glm::dot(this->vertices[i], direction);
                if (dot > max) {
                    max = dot;
                    result = this->vertices[i];
                }
            }
            return result;
        }

        glm::vec3 SphereGJKCollider::findFurtherPoint(glm::vec3 direction) const {
            return this->center_ + glm::normalize(direction) * this->scaledRadius_;
        }

        bool nextSimplex(Simplex &simplex, glm::vec3 &direction);

        PossibleCollision gjk(const GJKCollider &a, const GJKCollider &b) {
            glm::vec3 initial_direction = glm::vec3(1.0f, 0.0f, 0.0f);
            glm::vec3 support = a.findSupportPoint(b, initial_direction);
            // Create simplex
            Simplex simplex;
            simplex.push_front(support);
            glm::vec3 direction = -support;

            unsigned int iterations = 0;
            while (iterations < MAX_GJK_ITERATIONS) {
                iterations++;
                support = a.findSupportPoint(b, direction);
                if (glm::dot(support, direction) <= 0.0F) {
                    return {};
                }
                simplex.push_front(support);

                if (nextSimplex(simplex, direction)) {
                    return epa(a, b, simplex);
                }
            }
            // Exit after 25 iterations
            spdlog::warn("GJK: Exceeded 25 iterations");
            return PossibleCollision(); // No collision
        }

        void
        addIfNotPresent(std::vector<std::pair<unsigned int, unsigned int>> &edges,
                        const std::vector<unsigned int> &faces,
                        unsigned int a, unsigned int b) {
            auto reverse = std::find(
                    edges.begin(),
                    edges.end(),
                    std::make_pair(faces[b], faces[a])
            );
            if (reverse != edges.end()) {
                edges.erase(reverse);
            } else {
                edges.emplace_back(faces[a], faces[b]);
            }
        }

        std::pair<std::vector<glm::vec4>, unsigned int> calculateNormals(const std::vector<glm::vec3> &polytope,
                                                                         const std::vector<unsigned int> &faces) {
            std::vector<glm::vec4> normals;
            unsigned int minTriangle = 0;
            float minDistance = FLT_MAX;

            for (unsigned int i = 0; i < faces.size(); i += 3) {
                glm::vec3 a = polytope[faces[i]];
                glm::vec3 b = polytope[faces[i + 1]];
                glm::vec3 c = polytope[faces[i + 2]];

                glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
                float distance = glm::dot(normal, a);

                if (distance < 0.0f) {
                    normal *= -1;
                    distance *= -1;
                }

                normals.emplace_back(normal, distance);

                if (distance < minDistance) {
                    minTriangle = i / 3;
                    minDistance = distance;
                }
            }

            return {normals, minTriangle};
        }


        constexpr bool sameDirection(const glm::vec3 &dir, const glm::vec3 &ao) {
            return glm::dot(dir, ao) > 0.0f;
        }

        constexpr bool sameDirectionLvalue(const glm::vec3 dir, const glm::vec3 &ao) {
            return glm::dot(dir, ao) > 0.0f;
        }

        inline bool nextSimplexLine(Simplex &simplex, glm::vec3 &direction) {
            glm::vec3 const a = simplex[0];
            glm::vec3 const b = simplex[1];
            glm::vec3 ab = b - a;
            glm::vec3 ao = -a;
            if (sameDirection(ab, ao)) {
                direction = glm::cross(glm::cross(ab, ao), ab);
            } else {
                simplex = {a};
                direction = ao;
            }
            return false;
        }

        inline bool nextSimplexTriangle(Simplex &simplex, glm::vec3 &direction) {
            glm::vec3 const a = simplex[0];
            glm::vec3 const b = simplex[1];
            glm::vec3 const c = simplex[2];

            glm::vec3 const ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 ao = -a;

            glm::vec3 abc = glm::cross(ab, ac);

            if (sameDirectionLvalue(glm::cross(abc, ac), ao)) {
                if (sameDirection(ac, ao)) {
                    simplex = {a, c};
                    direction = glm::cross(glm::cross(ac, ao), ac);
                } else {
                    return nextSimplexLine(simplex = {a, b}, direction);
                }
            } else {
                if (sameDirectionLvalue(glm::cross(ab, abc), ao)) {
                    return nextSimplexLine(simplex = {a, b}, direction);
                } else {
                    if (sameDirection(abc, ao)) {
                        direction = abc;
                    } else {
                        simplex = {a, c, b};
                        direction = -abc;
                    }
                }
            }

            return false;
        }

        inline bool nextSimplexTetrahedron(Simplex &simplex, glm::vec3 &direction) {
            glm::vec3 const a = simplex[0];
            glm::vec3 const b = simplex[1];
            glm::vec3 const c = simplex[2];
            glm::vec3 const d = simplex[3];

            glm::vec3 const ab = b - a;
            glm::vec3 const ac = c - a;
            glm::vec3 const ad = d - a;
            glm::vec3 ao = -a;

            glm::vec3 abc = glm::cross(ab, ac);
            glm::vec3 acd = glm::cross(ac, ad);
            glm::vec3 adb = glm::cross(ad, ab);

            if (sameDirection(abc, ao))
                return nextSimplexTriangle(simplex = {a, b, c}, direction);
            if (sameDirection(acd, ao))
                return nextSimplexTriangle(simplex = {a, c, d}, direction);
            if (sameDirection(adb, ao))
                return nextSimplexTriangle(simplex = {a, d, b}, direction);
            return true;
        }

        inline bool nextSimplex(Simplex &simplex, glm::vec3 &direction) {
            switch (simplex.getSize()) {
                case 2:
                    return nextSimplexLine(simplex, direction);
                case 3:
                    return nextSimplexTriangle(simplex, direction);
                case 4:
                    return nextSimplexTetrahedron(simplex, direction);
            }
            return false;
        }

        PossibleCollision epa(const GJKCollider &a, const GJKCollider &b, const Simplex &simplex) {
            std::vector<glm::vec3> polytope(simplex.begin(), simplex.end());
            spdlog::debug("START EPA");
            spdlog::debug("EPA polytope size {}", polytope.size());
            spdlog::debug("EPA simplex {}", simplex.toString());
            std::vector<unsigned int> faces = {
                    0, 1, 2,
                    0, 3, 1,
                    0, 2, 3,
                    1, 3, 2
            };

            auto [normals, minFace] = calculateNormals(polytope, faces);
            glm::vec3 minNormal;
            float minDistance = FLT_MAX;

            unsigned int iterations = 0;
            while (minDistance == FLT_MAX) {
                spdlog::debug("EPA iteration {}", iterations);
//                spdlog::debug("EPA minFace {}", minFace);
//                spdlog::debug("EPA minDistance {}", minDistance);
//                spdlog::debug("EPA polytope size {}", polytope.size());
//                spdlog::debug("EPA faces size {}", faces.size());
//                spdlog::debug("EPA normals size {}", normals.size());
                minNormal = glm::vec3(normals[minFace]);
                minDistance = normals[minFace].w;

                if (iterations++ > GTK_EPA_MAX_ITER) {
                    break;
                }

                glm::vec3 support = a.findSupportPoint(b, minNormal);
                float supportDistance = glm::dot(minNormal, support);
                spdlog::debug("EPA supportDistance {}", supportDistance);
                spdlog::debug("EPA minDistance {}", minDistance);
                spdlog::debug("EPA supportDistance - minDistance {}", supportDistance - minDistance);
                if (abs(supportDistance - minDistance) > 0.001f) {
                    minDistance = FLT_MAX;
                    std::vector<std::pair<unsigned int, unsigned int>> uniqueEdges;
                    spdlog::debug("EPA faces size {}", faces.size());
                    spdlog::debug("EPA normals size {}", normals.size());
                    for (unsigned int i = 0; i < normals.size(); i++) {
                        if (sameDirection(normals[i], support)) {
                            unsigned int f = i * 3;
                            addIfNotPresent(uniqueEdges, faces, f, f + 1);
                            addIfNotPresent(uniqueEdges, faces, f + 1, f + 2);
                            addIfNotPresent(uniqueEdges, faces, f + 2, f);

                            faces[f + 2] = faces.back();
                            faces.pop_back();
                            faces[f + 1] = faces.back();
                            faces.pop_back();
                            faces[f] = faces.back();
                            faces.pop_back();

                            normals[i] = normals.back();
                            normals.pop_back();

                            i--;
                        }

                    }

                    if (uniqueEdges.size() == 0) {
                        break;
                    }

                    std::vector<unsigned int> newFaces;
                    for (auto [a, b]: uniqueEdges) {
                        newFaces.push_back(a);
                        newFaces.push_back(b);
                        newFaces.push_back(polytope.size());
                    }

                    polytope.push_back(support);

                    auto [newNormals, newMinFace] = calculateNormals(polytope, newFaces);

                    float newMinDistance = FLT_MAX;
                    for (unsigned int i = 0; i < normals.size(); i++) {
                        if (normals[i].w < newMinDistance) {
                            newMinDistance = normals[i].w;
                            minFace = i;
                        }
                    }

                    if (newNormals[newMinFace].w < newMinDistance) {
                        minFace = newMinFace + normals.size();
                    }

                    faces.insert(faces.end(), newFaces.begin(), newFaces.end());
                    normals.insert(normals.end(), newNormals.begin(), newNormals.end());

                }
            }

            if (minDistance == FLT_MAX) {
                spdlog::warn("EPA failed to find a collision");
                return {};
            }


            glm::vec3 contactNormal = glm::normalize(-minNormal);
            float penetrationDepth = minDistance + 0.001f;

            return PossibleCollision(penetrationDepth, contactNormal);

        }

        std::string Simplex::toString() const {
            std::string result = "";

            for (int i = 0; i < size_; i++) {
                result += "-" + std::to_string(i) + ": " + glm::to_string(points_[i]) + " ";
            };

            return result;

        }
    }

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionBoxSphere(glm::mat4 box_transform, glm::mat4 &sphere_transform) {
        GJK_EPA::CubeGJKCollider const a(box_transform);
        GJK_EPA::SphereGJKCollider const b(sphere_transform);

        return GJK_EPA::gjk(a, b);
    }

    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 box_transform, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionBoxBox(glm::mat4 box1_t, glm::mat4 box2_t) {
        GJK_EPA::CubeGJKCollider const a(box1_t);
        GJK_EPA::CubeGJKCollider const b(box2_t);

        return GJK_EPA::gjk(b, a);
    }

    collisions::PossibleCollision checkCollisionSphereSphere(glm::mat4 &s1_t, float s1_r, glm::mat4 &s2_t, float s2_r) {
        glm::vec4 s1_center = s1_t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 s2_center = s2_t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        float radii = s1_r + s2_r;
        float distance = glm::length(s1_center - s2_center);
        if (distance > radii) {
            return collisions::PossibleCollision();
        }

        glm::vec3 normal = glm::normalize(glm::vec3(s2_center - s1_center));
        float penetration = radii - distance;

        // TODO: Treat case where they are exacly at the same space as an edge case.

        return collisions::PossibleCollision(penetration, normal, true);
    }

}