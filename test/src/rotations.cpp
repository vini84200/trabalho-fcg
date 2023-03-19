#include "gtest/gtest.h"
#include "test_utils.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/trigonometric.hpp"

#include "utils/matrices.h"

// Use https://www.andre-gaschler.com/rotationconverter/ to convert between quaternions and rotation matrices

namespace entre_portais::test {

    TEST (Matrices, Multiplication) {
        glm::mat4 M = glm::mat4(1.0f);
        glm::vec4 v = glm::vec4(1.0f);
        auto r = M * v;
        EXPECT_EQ(r, glm::vec4(1.0f));
    }

// Rotate a vector by a quaternion
    TEST (Rotations, RotateVectorByQuaternion) {
        glm::vec3 v = glm::vec3(1.0f, 0.0f, 0.0f);
        // Quat: w x y z
        glm::quat q = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 r = q * v;
        EXPECT_EQ(r, glm::vec3(1.0f, 0.0f, 0.0f));
        // Rotate 90 degrees counter-clockwise around the y axis
        q = glm::quat(0.7071068f, 0.0f, 0.7071068f, 0.0f);
        r = q * v;
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, r, glm::vec3(0.0f, 0.0f, -1.0f), 0.0001f);
    }

    TEST(Rotations, RotationMatrixFromQuat) {
        // Rotate 90 degrees around the y axis
        glm::quat q = glm::quat(0.7071068f, 0.0f, 0.7071068f, 0.0f);
        glm::mat4 m = matrices::RotationFromQuat(q);
        glm::mat4 expected = matrices::Matrix(0, 0, 1, 0,
                                              0, 1, 0, 0,
                                              -1, 0, 0, 0,
                                              0, 0, 0, 1);

        EXPECT_PRED_FORMAT3(utils::assert_mat4_equal, m, expected, 0.0001f);
    }

    TEST(Rotations, RotationMatrixFromQuatNotNormalized) {
        // Rotate 103.7 degrees around the y axis
        glm::quat q = glm::quat(1.57079632679f, 0.0f, 2.0f, 0.0f);
        glm::mat4 m = matrices::RotationFromQuat(q);
        glm::mat4 expected = matrices::Matrix(-0.2369729, 0.0000000, 0.9715163, 0.f,
                                              0.0000000, 1.0000000, 0.0000000, 0.f,
                                              -0.9715163, 0.0000000, -0.2369729, 0.f,
                                              0.f, 0.f, 0.f, 1.f);
        EXPECT_PRED_FORMAT3(utils::assert_mat4_equal, m, expected, 0.0001f);
    }

    TEST(Rotations, QuaternionFromEuler90Y) {
        // Rotate 90 degrees around the y axis
        glm::vec3 euler = glm::vec3(0.0f, M_PI_2f, 0.0f);
        glm::quat q = glm::quat(euler);
        glm::quat expected = glm::quat(0.7071068f, 0.0f, 0.7071068f, 0.0f);

        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, q, expected, 0.0001f);

    }

    TEST(Rotations, QuaternionFromEulerAnyVector) {
        // Rotate 90 degrees around the y axis
        glm::vec3 euler = glm::vec3(4.f, 5.f, 6.f);
        glm::quat q = glm::quat(euler);
        glm::quat expected = glm::quat(-0.253261, 0.7563338, 0.1437571, 0.5857917); // ZYX
        glm::vec3 new_euler = glm::eulerAngles(q);
        glm::vec3 new_euler2 = glm::eulerAngles(expected);
        glm::vec3 expected_euler = glm::vec3(-2.2831853, -1.2831854, -0.2831853); // ZYX

        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, q, expected, 0.0001f);
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, new_euler, new_euler2, 0.0001f);
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, new_euler, expected_euler, 0.0001f);
    }

    TEST(Rotations, QuaternionFromRotationMatrix90Y) {
        // Rotate 90 degrees around the y axis
        glm::mat4 m = matrices::Matrix(0, 0, 1, 0,
                                       0, 1, 0, 0,
                                       -1, 0, 0, 0,
                                       0, 0, 0, 1);
        glm::quat q = matrices::RotationFromMatrix(m);
        glm::quat expected = glm::quat(0.7071068f, 0.0f, 0.7071068f, 0.0f);

        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, q, expected, 0.0001f);
    }

    TEST(Rotations, QuaternionFromRotationMatrixNotPure) {
        // Scale
        glm::mat4 s = matrices::Matrix(2, 0, 0, 0,
                                       0, 2, 0, 0,
                                       0, 0, 2, 0,
                                       0, 0, 0, 1);
        // Rotate 90 degrees around the y axis
        glm::mat4 m = matrices::Matrix(0, 0, 1, 0,
                                       0, 1, 0, 0,
                                       -1, 0, 0, 0,
                                       0, 0, 0, 1);
        // Translate
        glm::mat4 t = matrices::Matrix(1, 0, 0, 3,
                                       0, 1, 0, -2,
                                       0, 0, 1, 7,
                                       0, 0, 0, 1);
        glm::quat q = matrices::RotationFromMatrix(t * m * s);
        glm::quat expected = glm::quat(0.7071068f, 0.0f, 0.7071068f, 0.0f);

        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, q, expected, 0.0001f);
    }
} // namespace entre_portais::test