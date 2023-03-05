#include "entrePortaisEngine/physics/collisions.hpp"
#include "spdlog/spdlog.h"
#include "glm/gtx/string_cast.hpp"
#include <vector>

#define GTK_EPA_MAX_ITER 32
namespace entre_portais::collisions {

    namespace GJK_EPA {
        // FONTE: https://github.com/DanielChappuis/reactphysics3d/blob/master/src/collision/narrowphase/GJK/GJKAlgorithm.cpp
        // FONTE: https://github.com/DanielChappuis/reactphysics3d/blob/master/src/collision/narrowphase/GJK/VoronoiSimplex.cpp
        // ESSA PARTE DO CÓDIGO FOI ADAPTADA DO CÓDIGO DO AUTOR ACIMA CITADO
        VoronoiSimplex::VoronoiSimplex()
                : numPoints_(0), recomputeClosestPoint_(false), isClosestPointValid_(false) {};

        void VoronoiSimplex::addPoint(const glm::vec3 &point, const glm::vec3 &pointA, const glm::vec3 &pointB) {
            assert(numPoints_ < 4);
            points_[numPoints_] = point;
            supportPointsA_[numPoints_] = pointA;
            supportPointsB_[numPoints_] = pointB;

            numPoints_++;
            recomputeClosestPoint_ = true;
        }

        void VoronoiSimplex::removePoint(int index) {
            assert(numPoints_ > 0);
            assert(index >= 0 && index < numPoints_);

            points_[index] = points_[numPoints_ - 1];
            supportPointsA_[index] = supportPointsA_[numPoints_ - 1];
            supportPointsB_[index] = supportPointsB_[numPoints_ - 1];

            numPoints_--;
        }

        void VoronoiSimplex::reduceSimplex(int bitUsedVertices) {
            if ((numPoints_ >= 4) && (bitUsedVertices & 0x08) == 0) {
                removePoint(3);
            }
            if ((numPoints_ >= 3) && (bitUsedVertices & 0x04) == 0) {
                removePoint(2);
            }
            if ((numPoints_ >= 2) && (bitUsedVertices & 0x02) == 0) {
                removePoint(1);
            }
            if ((numPoints_ >= 1) && (bitUsedVertices & 0x01) == 0) {
                removePoint(0);
            }
        }

        bool VoronoiSimplex::isPointInSimplex(const glm::vec3 &point) const {
            for (int i = 0; i < numPoints_; i++) {
                float distance2 = glm::distance2(point, points_[i]);
                if (distance2 <= epsilon) {
                    return true;
                }
            }
            return false;
        }

        int VoronoiSimplex::getSimplex(glm::vec3 *supportPointsA, glm::vec3 *supportPointsB, glm::vec3 *points) const {
            for (int i = 0; i < numPoints_; i++) {
                supportPointsA[i] = supportPointsA_[i];
                supportPointsB[i] = supportPointsB_[i];
                points[i] = points_[i];
            }
            return numPoints_;
        }

        bool VoronoiSimplex::isAffinelyDependent() const {
            assert(numPoints_ >= 0 && numPoints_ <= 4);
            switch (numPoints_) {
                case 0:
                    return false;
                case 1:
                    return true;
                case 2:
                    return glm::distance2(points_[0], points_[1]) <= epsilon;
                case 3:
                    return glm::length2(glm::cross(points_[1] - points_[0], points_[2] - points_[0])) <= epsilon;
                case 4:
                    return std::abs(
                            glm::dot(points_[1] - points_[0],
                                     glm::cross(points_[2] - points_[0], points_[3] - points_[0]))
                    ) <= epsilon;
                default:
                    assert(false);
                    return false;

            }
        }

        void VoronoiSimplex::computeClosestPointsOfAandB(glm::vec3 &pointA, glm::vec3 &pointB) const {
            pointA = closestSupportPointA_;
            pointB = closestSupportPointB_;
        }

        bool VoronoiSimplex::recomputeClosestPoint() {
            assert(numPoints_ >= 0 && numPoints_ <= 4);
            if (recomputeClosestPoint_) {
                recomputeClosestPoint_ = false;
                switch (numPoints_) {
                    case 0:
                        isClosestPointValid_ = false;
                        break;
                    case 1: {
                        closestSupportPointA_ = supportPointsA_[0];
                        closestSupportPointB_ = supportPointsB_[0];
                        closestPoint_ = points_[0];
                        setBarycentricCoordinates(1, 0, 0, 0);
                        isClosestPointValid_ = checkClosestPointIsValid();
                    }
                        break;
                    case 2: {
                        int bitsUsedPoints = 0;
                        float t;
                        computeClosestPointOnSegment(points_[0], points_[1], bitsUsedPoints, t);
                        closestSupportPointA_ = supportPointsA_[0] + t * (supportPointsA_[1] - supportPointsA_[0]);
                        closestSupportPointB_ = supportPointsB_[0] + t * (supportPointsB_[1] - supportPointsB_[0]);
                        closestPoint_ = closestSupportPointA_ - closestSupportPointB_;
                        setBarycentricCoordinates(1 - t, t, 0, 0);
                        isClosestPointValid_ = checkClosestPointIsValid();

                        reduceSimplex(bitsUsedPoints);
                    }
                        break;
                    case 3: {
                        int bitsUsedPoints = 0;
                        glm::vec3 barycentricCoordinates;
                        computeClosestPointOnTriangle(points_[0], points_[1], points_[2], bitsUsedPoints,
                                                      barycentricCoordinates);
                        closestSupportPointA_ = supportPointsA_[0] * barycentricCoordinates.x +
                                                supportPointsA_[1] * barycentricCoordinates.y +
                                                supportPointsA_[2] * barycentricCoordinates.z;
                        closestSupportPointB_ = supportPointsB_[0] * barycentricCoordinates.x +
                                                supportPointsB_[1] * barycentricCoordinates.y +
                                                supportPointsB_[2] * barycentricCoordinates.z;
                        closestPoint_ = closestSupportPointA_ - closestSupportPointB_;
                        setBarycentricCoordinates(barycentricCoordinates.x, barycentricCoordinates.y,
                                                  barycentricCoordinates.z, 0);
                        isClosestPointValid_ = checkClosestPointIsValid();

                        reduceSimplex(bitsUsedPoints);
                    }
                        break;
                    case 4: {
                        int bitsUsedVertices = 0;
                        glm::vec2 baryCoordsAB;
                        glm::vec2 baryCoordsCD;
                        bool isDegenerate;

                        bool isOutside = computeClosestPointOnTetrahedron(points_[0], points_[1], points_[2],
                                                                          points_[3],
                                                                          bitsUsedVertices, baryCoordsAB, baryCoordsCD,
                                                                          isDegenerate);

                        if (isOutside) {

                            closestSupportPointA_ =
                                    baryCoordsAB.x * supportPointsA_[0] + baryCoordsAB.y * supportPointsA_[1] +
                                    baryCoordsCD.x * supportPointsA_[2] + baryCoordsCD.y * supportPointsA_[3];
                            closestSupportPointB_ =
                                    baryCoordsAB.x * supportPointsB_[0] + baryCoordsAB.y * supportPointsB_[1] +
                                    baryCoordsCD.x * supportPointsB_[2] + baryCoordsCD.y * supportPointsB_[3];
                            closestPoint_ = closestSupportPointA_ - closestSupportPointB_;

                            setBarycentricCoordinates(baryCoordsAB.x, baryCoordsAB.y, baryCoordsCD.x, baryCoordsCD.y);

                            reduceSimplex(bitsUsedVertices);
                        } else {

                            if (isDegenerate) {
                                isClosestPointValid_ = false;
                            } else {


                                setBarycentricCoordinates(0.0, 0.0, 0.0, 0.0);

                                closestSupportPointA_ = glm::vec3(0.0, 0.0, 0.0);
                                closestSupportPointB_ = glm::vec3(0.0, 0.0, 0.0);
                                closestPoint_ = glm::vec3(0.0, 0.0, 0.0);

                                isClosestPointValid_ = true;
                            }
                            break;
                        }

                        isClosestPointValid_ = checkClosestPointIsValid();
                    }
                        break;
                    default:
                        assert(false);
                        return false;
                }
            }
            return isClosestPointValid_;
        }

        void VoronoiSimplex::computeClosestPointOnSegment(const glm::vec3 &pointA, const glm::vec3 &pointB,
                                                          int &bitsUsedPoints, float &t) const {
            glm::vec3 AP = -pointA;
            glm::vec3 AB = pointB - pointA;
            float APDotAB = glm::dot(AP, AB);

            if (APDotAB > 0.0f) {
                float lenAB2 = glm::dot(AB, AB);

                if (APDotAB < lenAB2) {
                    t = APDotAB / lenAB2;
                    bitsUsedPoints = 0x03;
                } else {
                    t = 1.0f;
                    bitsUsedPoints = 0x02;
                }
            } else {
                t = 0.0f;
                bitsUsedPoints = 0x01;
            }
        }

        void VoronoiSimplex::computeClosestPointOnTriangle(const glm::vec3 &pointA, const glm::vec3 &pointB,
                                                           const glm::vec3 &pointC, int &bitsUsedPoints,
                                                           glm::vec3 &barycentricCoordinates) const {
            glm::vec3 AB = pointB - pointA;
            glm::vec3 AC = pointC - pointA;
            glm::vec3 AP = -pointA;
            float d1 = glm::dot(AB, AP);
            float d2 = glm::dot(AC, AP);

            if (d1 <= 0.0f && d2 <= 0.0f) {
                barycentricCoordinates = glm::vec3(1.0f, 0.0f, 0.0f);
                bitsUsedPoints = 0x01;
                return;
            }

            glm::vec3 BP = -pointB;
            float d3 = glm::dot(AB, BP);
            float d4 = glm::dot(AC, BP);

            if (d3 >= 0.0f && d4 <= d3) {
                barycentricCoordinates = glm::vec3(0.0f, 1.0f, 0.0f);
                bitsUsedPoints = 0x02;
                return;
            }

            float vc = d1 * d4 - d3 * d2;
            if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
                assert(std::abs(d1 - d3) > std::numeric_limits<float>::epsilon());
                float v = d1 / (d1 - d3);
                barycentricCoordinates = glm::vec3(1.0f - v, v, 0.0f);
                bitsUsedPoints = 0x03;
                return;
            }

            glm::vec3 CP = -pointC;
            float d5 = glm::dot(AB, CP);
            float d6 = glm::dot(AC, CP);

            if (d6 >= 0.0f && d5 <= d6) {
                barycentricCoordinates = glm::vec3(0.0f, 0.0f, 1.0f);
                bitsUsedPoints = 0x04;
                return;
            }

            float vb = d5 * d2 - d1 * d6;
            if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
                assert(std::abs(d2 - d6) > std::numeric_limits<float>::epsilon());
                float w = d2 / (d2 - d6);
                barycentricCoordinates = glm::vec3(1.0f - w, 0.0f, w);
                bitsUsedPoints = 0x05;
                return;
            }

            float va = d3 * d6 - d5 * d4;
            if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
                assert(std::abs(d4 - d3) > std::numeric_limits<float>::epsilon());
                assert(std::abs(d5 - d6) > std::numeric_limits<float>::epsilon());
                assert(std::abs((d4 - d3) + (d5 - d6)) > std::numeric_limits<float>::epsilon());
                float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
                barycentricCoordinates = glm::vec3(0.0f, 1.0f - w, w);
                bitsUsedPoints = 0x06;
                return;
            }

            float denom = 1.0f / (va + vb + vc);
            float v = vb * denom;
            float w = vc * denom;
            barycentricCoordinates = glm::vec3(1.0f - v - w, v, w);
            bitsUsedPoints = 0x07;
            return;
        }

        bool VoronoiSimplex::computeClosestPointOnTetrahedron(const glm::vec3 &a, const glm::vec3 &b,
                                                              const glm::vec3 &c, const glm::vec3 &d,
                                                              int &bitsUsedPoints, glm::vec2 &baryCoordsAB,
                                                              glm::vec2 &baryCoordsCD, bool &isDegenerate) const {
            isDegenerate = false;
            bitsUsedPoints = 0x0F;
            baryCoordsAB = glm::vec2(0.0f, 0.0f);
            baryCoordsCD = glm::vec2(0.0f, 0.0f);

            int isOriginOutsideABC = isPointOutsideOfPlane(a, b, c, d);
            int isOriginOutsideACD = isPointOutsideOfPlane(a, c, d, b);
            int isOriginOutsideADB = isPointOutsideOfPlane(a, d, b, c);
            int isOriginOutsideBDC = isPointOutsideOfPlane(b, d, c, a);

            if (isOriginOutsideABC < 0 && isOriginOutsideACD < 0 && isOriginOutsideADB < 0 && isOriginOutsideACD < 0) {
                isDegenerate = true;
                return false;
            }

            if (isOriginOutsideABC == 0 && isOriginOutsideACD == 0 && isOriginOutsideADB == 0 &&
                isOriginOutsideACD == 0) {
                return true;
            }

            float minDist = std::numeric_limits<float>::max();
            int tempUsedPoints;
            glm::vec3 triangleBaryCoords;

            if (isOriginOutsideABC) {
                computeClosestPointOnTriangle(a, b, c, tempUsedPoints, triangleBaryCoords);
                glm::vec3 closestPoint = a * triangleBaryCoords.x + b * triangleBaryCoords.y + c * triangleBaryCoords.z;
                float dist2 = glm::dot(closestPoint, closestPoint);
                if (dist2 < minDist) {
                    minDist = dist2;
                    baryCoordsAB = glm::vec2(triangleBaryCoords.x, triangleBaryCoords.y);
                    baryCoordsCD = glm::vec2(triangleBaryCoords.z, 0.0f);
                    bitsUsedPoints = tempUsedPoints;
                }
            }

            if (isOriginOutsideACD) {
                computeClosestPointOnTriangle(a, c, d, tempUsedPoints, triangleBaryCoords);
                glm::vec3 closestPoint = a * triangleBaryCoords.x + c * triangleBaryCoords.y + d * triangleBaryCoords.z;
                float dist2 = glm::dot(closestPoint, closestPoint);

                if (dist2 < minDist) {
                    minDist = dist2;
                    baryCoordsAB = glm::vec2(triangleBaryCoords.x, 0.0f);
                    baryCoordsCD = glm::vec2(triangleBaryCoords.y, triangleBaryCoords.z);
                    bitsUsedPoints = mapTriangleUsedVerticesToTetrahedron(tempUsedPoints, 0x00, 0x02, 0x03);
                }
            }

            if (isOriginOutsideADB) {
                computeClosestPointOnTriangle(a, d, b, tempUsedPoints, triangleBaryCoords);
                glm::vec3 closestPoint = a * triangleBaryCoords.x + d * triangleBaryCoords.y + b * triangleBaryCoords.z;
                float dist2 = glm::dot(closestPoint, closestPoint);
                if (dist2 < minDist) {
                    minDist = dist2;
                    baryCoordsAB = glm::vec2(triangleBaryCoords.x, triangleBaryCoords.z);
                    baryCoordsCD = glm::vec2(0.0f, triangleBaryCoords.y);
                    bitsUsedPoints = mapTriangleUsedVerticesToTetrahedron(tempUsedPoints, 0x00, 0x03, 0x01);
                }
            }

            if (isOriginOutsideBDC) {
                computeClosestPointOnTriangle(b, d, c, tempUsedPoints, triangleBaryCoords);
                glm::vec3 closestPoint = b * triangleBaryCoords.x + d * triangleBaryCoords.y + c * triangleBaryCoords.z;
                float dist2 = glm::dot(closestPoint, closestPoint);
                if (dist2 < minDist) {
                    minDist = dist2;
                    baryCoordsAB = glm::vec2(0.0f, triangleBaryCoords.x);
                    baryCoordsCD = glm::vec2(triangleBaryCoords.z, triangleBaryCoords.y);
                    bitsUsedPoints = mapTriangleUsedVerticesToTetrahedron(tempUsedPoints, 0x01, 0x03, 0x02);
                }
            }

            return true;

        }

        int VoronoiSimplex::mapTriangleUsedVerticesToTetrahedron(int triangleUsedVertices, int first, int second,
                                                                 int third) const {
            assert(triangleUsedVertices >= 0 && triangleUsedVertices <= 0x07);
            int tetrahedronUsedVertices = (((1 & triangleUsedVertices) != 0 << first) |
                                           ((2 & triangleUsedVertices) != 0 << second) |
                                           ((4 & triangleUsedVertices) != 0 << third));
            assert(tetrahedronUsedVertices >= 0 && tetrahedronUsedVertices <= 0x0E);
            return tetrahedronUsedVertices;
        }

        int VoronoiSimplex::isPointOutsideOfPlane(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c,
                                                  const glm::vec3 &d) const {
            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 normal = glm::cross(ab, ac);

            float signp = glm::dot(-a, normal);
            float signd = glm::dot(d - a, normal);
            if (signd * signp < epsilon2) {
                return -1;
            }

            return signp * signd < 0.f;
        }

        void VoronoiSimplex::backupClosestPoint(glm::vec3 &point) {
            point = closestPoint_;
        }

        float VoronoiSimplex::getMaxLength2() const {
            float maxLength2 = 0.f;
            for (int i = 0; i < numPoints_; ++i) {
                float length2 = glm::dot(points_[i], points_[i]);
                if (length2 > maxLength2) {
                    maxLength2 = length2;
                }
            }
            return maxLength2;
        }

        // GJK Algorithm


        PossibleCollision GJKAlgorithm::testCollision(const GJKCollider &colliderA, const GJKCollider &colliderB,
                                                      GJKAlgorithm::GJKResult &result,
                                                      const glm::mat4 &transformA, const glm::mat4 &transformB) {
            glm::vec3 supportA, supportB, w, pA, pB;
            float vDotw, prevDist2;
            bool contactFound = false;

            glm::mat4 transformAInv = glm::inverse(transformA);
            glm::mat4 bodyBToBodyA = transformAInv * transformB;

            glm::quat rotateToBodyB =
                    inverse(matrices::RotationFromMatrix(transformB)) * matrices::RotationFromMatrix(transformA);

            float margin = colliderA.getMargin() + colliderB.getMargin();
            float margin2 = margin * margin;
            assert(margin > 0.f);

            VoronoiSimplex simplex;

            glm::vec3 v = glm::vec3(0.f, 1.f, 0.f);

            float dist2 = std::numeric_limits<float>::max();

            bool noIntersection = false;

            do {
                supportA = colliderA.getLocalSupportPointWithoutMargin(-v);
                supportB = bodyBToBodyA * glm::vec4(colliderB.getLocalSupportPointWithoutMargin(rotateToBodyB * v), 1);
                w = supportA - supportB;
                vDotw = glm::dot(v, w);

                if (vDotw > 0.f && vDotw * vDotw > margin2 * dist2) {
                    noIntersection = true;
                    result = GJKResult::Separated;
                    break;
                }

                if (simplex.isPointInSimplex(w) || dist2 - vDotw <= dist2 * 0.0001f) {
                    contactFound = true;
                    break;
                }

                simplex.addPoint(w, supportA, supportB);

                if (simplex.isAffinelyDependent()) {
                    contactFound = true;
                    break;
                }

                if (!simplex.computeClosestPoint(v)) {
                    contactFound = true;
                    break;
                }

                prevDist2 = dist2;
                dist2 = glm::dot(v, v);

                if (prevDist2 - dist2 <= std::numeric_limits<float>::epsilon() * prevDist2) {
                    simplex.backupClosestPoint(v);
                    dist2 = glm::dot(v, v);
                    contactFound = true;
                    break;
                }

            } while (!simplex.isFull() && dist2 > std::numeric_limits<float>::epsilon() * simplex.getMaxLength2());

            if (noIntersection) {
                return {};
            }

            if (contactFound && dist2 > std::numeric_limits<float>::epsilon()) {

                simplex.computeClosestPointsOfAandB(pA, pB);

                float distance = glm::length(dist2);
                assert(distance > 0.f);
                pA = (pA - (colliderA.getMargin() / distance) * v);
                pB = glm::inverse(bodyBToBodyA) * glm::vec4((pB + (colliderB.getMargin() / distance) * v), 1);

                // Compute the contact information
                glm::vec3 normal = matrices::RotationFromMatrix(transformA) * (glm::normalize(-v));
                float penetration = margin - distance;

                if (penetration < 0.f) {
                    result = GJKResult::Separated;
                    return {};
                }

                if (glm::length2(normal) < std::numeric_limits<float>::epsilon()) {
                    result = GJKResult::Separated;
                    return {};
                }

                // Compute the contact point

                result = GJKResult::CollideInMargin;
                return PossibleCollision::contact(pA, pB, normal, penetration);
            }

            result = GJKResult::Penetrating;
            return PossibleCollision::penetration();


        }
    }

    collisions::PossibleCollision checkCollisionSpherePoint(glm::mat4 &sphereModel, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionBoxSphere(glm::mat4 box_transform, glm::mat4 &sphere_transform) {
        GJK_EPA::CubeGJKCollider a({1 / 2.f, 1 / 2.f, 1 / 2.f});
        GJK_EPA::SphereGJKCollider b(1 / 2.f);

        GJK_EPA::GJKAlgorithm algo;
        GJK_EPA::GJKAlgorithm::GJKResult result;
        auto collision =
                algo.testCollision(a, b, result, box_transform, sphere_transform);

        spdlog::info("Collision: {}", collision.isContact());
        spdlog::info("Penetration: {}", collision.isPenetration());
        spdlog::info("Distance: {}", collision.depth);
        spdlog::info("Normal: {}", glm::to_string(collision.normal));
        spdlog::info("Point A: {}", glm::to_string(collision.pointA));
        spdlog::info("Point B: {}", glm::to_string(collision.pointB));


        if (collision.isContact()) {
            return collision;
        }

        if (collision.isPenetration()) {
            // FIXME: Use another ( :( ) algorithm to find the penetration depth
            // and the contact point
            spdlog::warn("Penetration depth not implemented for box-sphere collision");
            return collision;
        }

        return collisions::PossibleCollision();

    }

    collisions::PossibleCollision checkCollisionBoxPoint(glm::mat4 box_transform, glm::vec4 point) {
        return collisions::PossibleCollision();
    }

    collisions::PossibleCollision checkCollisionBoxBox(glm::mat4 box1_t, glm::mat4 box2_t) {
        GJK_EPA::CubeGJKCollider a({1 / 2.f, 1 / 2.f, 1 / 2.f});
        GJK_EPA::CubeGJKCollider b({1 / 2.f, 1 / 2.f, 1 / 2.f});
        a.setMargin(0.1f);
        b.setMargin(0.1f);

        // FIXME: USE SAT as in https://www.gdcvault.com/play/1017646/Physics-for-Game-Programmers-The
        // to check if the boxes are not colliding
        GJK_EPA::GJKAlgorithm algo;
        GJK_EPA::GJKAlgorithm::GJKResult result;
        auto collision =
                algo.testCollision(a, b, result, box1_t, box2_t);

        if (collision.isContact()) {
            return collision;
        }

        if (collision.isPenetration()) {
            // FIXME: Use another ( :( ) algorithm to find the penetration depth
            // and the contact point
            spdlog::warn("Penetration depth not implemented for box-box collision");
            return collision;
        }
        return collisions::PossibleCollision();
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
        if (isnanf(normal.x) || isnanf(normal.y) || isnanf(normal.z)) {
            spdlog::warn("Normal is nan: {}", glm::to_string(normal));
            return collisions::PossibleCollision::contact(s1_center, s2_center, glm::vec3(0, 1, 0), penetration);

        }

        // TODO: Treat case where they are exacly at the same space as an edge case.

        if (penetration < 0.f) {
            return collisions::PossibleCollision();
        } else if (glm::length2(normal) < std::numeric_limits<float>::epsilon()) {
            return collisions::PossibleCollision();
        } else {
            return collisions::PossibleCollision::contact(s1_center, s2_center, normal, penetration);
        }
    }

}