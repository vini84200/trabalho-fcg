#define NDEBUG 1

#include "gtest/gtest.h"
#include "test_utils.hpp"
#include "entrePortaisEngine/Transform.hpp"

namespace entre_portais::test {
    TEST(Transforms, Constructor) {
        Transform t;
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getPosition(), glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getScale(), glm::vec3(1.0f, 1.0f, 1.0f), 0.0001f);
        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, t.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 0.0001f);
    }

    TEST(Transforms, SetPosition) {
        Transform t;
        t.setPosition(glm::vec3(1.0f, 2.0f, 3.0f));
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getPosition(), glm::vec3(1.0f, 2.0f, 3.0f), 0.0001f);
    }

    TEST(Transforms, SetScale) {
        Transform t;
        t.setScale(glm::vec3(1.0f, 2.0f, 3.0f));
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getScale(), glm::vec3(1.0f, 2.0f, 3.0f), 0.0001f);
    }

    TEST(Transforms, SetRotation) {
        Transform t;
        t.setRotation(glm::quat(1.0f, 2.0f, 3.0f, 4.0f));
        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, t.getRotation(),
                            glm::quat(0.1825742, 0.3651484, 0.5477226, 0.7302967), 0.0001f);
    }

    TEST(Transforms, SetRotationZYX) {
        Transform t;
        t.setRotationZYX(glm::vec3(1.0f, 2.0f, 3.0f));
        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, t.getRotation(),
                            glm::quat(0.4359528, -0.718287, 0.3106225, 0.4444351), 0.0001f);
    }

    TEST(Transforms, GetForward) {
        Transform t;
        t.setRotationZYX(glm::vec3(0.0f, 0.0f, 0.0f));
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getForward(), glm::vec3(0.0f, 0.0f, -1.0f), 0.0001f);
        t.setRotationZYX(glm::vec3(0.0f, M_PI_2f, 0.0f));
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getForward(), glm::vec3(-1.0f, 0.0f, 0.0f), 0.0001f);
        t.setRotationZYX(glm::vec3(0.0f, M_PI, 0.0f));
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getForward(), glm::vec3(0.0f, 0.0f, 1.0f), 0.0001f);
        t.setRotationZYX(glm::vec3(0.0f, 3.0f * M_PI_2f, 0.0f));
        EXPECT_PRED_FORMAT3(utils::assert_vec3_equal, t.getForward(), glm::vec3(1.0f, 0.0f, 0.0f), 0.0001f);
    }

    TEST(Transforms, RotateByQuaternion) {
        Transform t;
        t.setRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
        t.rotateBy(glm::quat(0.0f, 1.0f, 0.0f, 0.0f));
        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, t.getRotation(), glm::quat(0.0f, 1.0f, 0.0f, 0.0f), 0.0001f);
        //TODO: Fix this test
//        t.rotateBy(glm::quat(0.0f, 0.0f, 1.0f, 0.0f));
//        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, t.getRotation(), glm::quat(0.0f, 0.0f, 1.0f, 0.0f), 0.0001f);
//        t.rotateBy(glm::quat(0.0f, 0.0f, 0.0f, 1.0f));
//        EXPECT_PRED_FORMAT3(utils::assert_quat_equal, t.getRotation(), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), 0.0001f);
    }


} // entre_portais::test