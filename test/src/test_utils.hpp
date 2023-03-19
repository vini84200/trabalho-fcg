#ifndef ENTREPORTAIS_TEST_UTILS_HPP
#define ENTREPORTAIS_TEST_UTILS_HPP

#include <glm/glm.hpp>
#include <gtest/gtest.h>
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"

namespace entre_portais::test::utils {
    bool is_vec3_equal(const glm::vec3 a, const glm::vec3 b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }

    bool is_vec4_equal(const glm::vec4 a, const glm::vec4 b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }

    bool is_mat4_equal(const glm::mat4 a, const glm::mat4 b, float epsilon = 0.0001f) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (!glm::epsilonEqual(a[i][j], b[i][j], epsilon)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_quat_equal(const glm::quat a, const glm::quat b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }

    testing::AssertionResult assert_vec3_equal(const char *a_expr, const char *b_expr, const char *epsilon_expr,
                                               const glm::vec3 a, const glm::vec3 b, float epsilon = 0.0001f) {
        if (is_vec3_equal(a, b, epsilon)) {
            return testing::AssertionSuccess();
        } else {
            for (int i = 0; i < 3; i++) {
                if (!glm::epsilonEqual(a[i], b[i], epsilon)) {
                    return testing::AssertionFailure()
                            << a_expr << " and " << b_expr << " are not equal. "
                            << a_expr << "[" << i << "] = " << a[i] << " != " << b[i] << " = " << b_expr << "["
                            << i << "]. "
                            << "Epsilon = " << epsilon_expr << " = " << epsilon << ".";
                }
            }
            return testing::AssertionFailure()
                    << a_expr << " and " << b_expr << " are not equal. "
                    << "Epsilon = " << epsilon_expr << " = " << epsilon << ".";
        }
    }

    testing::AssertionResult assert_vec4_equal(const char *a_expr, const char *b_expr, const char *epsilon_expr,
                                               const glm::vec4 a, const glm::vec4 b, float epsilon = 0.0001f) {
        if (is_vec4_equal(a, b, epsilon)) {
            return testing::AssertionSuccess();
        } else {
            for (int i = 0; i < 4; i++) {
                if (!glm::epsilonEqual(a[i], b[i], epsilon)) {
                    return testing::AssertionFailure()
                            << a_expr << " and " << b_expr << " are not equal. "
                            << a_expr << "[" << i << "] = " << a[i] << " != " << b[i] << " = " << b_expr << "["
                            << i << "]. "
                            << "Epsilon = " << epsilon_expr << " = " << epsilon << ".";
                }
            }
            return testing::AssertionFailure()
                    << a_expr << " and " << b_expr << " are not equal. "
                    << "Epsilon = " << epsilon_expr << " = " << epsilon << ".";
        }
    }

    testing::AssertionResult assert_mat4_equal(const char *a_expr, const char *b_expr, const char *epsilon_expr,
                                               const glm::mat4 a, const glm::mat4 b, float epsilon = 0.0001f) {
        if (is_mat4_equal(a, b, epsilon)) {
            return testing::AssertionSuccess();
        } else {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (!glm::epsilonEqual(a[i][j], b[i][j], epsilon)) {
                        return testing::AssertionFailure()
                                << a_expr << " and " << b_expr << " are not equal. "
                                << a_expr << "[" << i << "][" << j << "] = " << a[i][j] << " != " << b[i][j]
                                << " = " << b_expr << "[" << i << "][" << j << "]. "
                                << "Epsilon = " << epsilon_expr << " = " << epsilon << ".";
                    }
                }
            }
            return testing::AssertionFailure()
                    << a_expr << " and " << b_expr << " are not equal. "
                    << "Epsilon = " << epsilon_expr << " = " << epsilon << ".";
        }
    }

    testing::AssertionResult assert_quat_equal(const char *a_expr, const char *b_expr, const char *epsilon_expr,
                                               const glm::quat a, const glm::quat b, float epsilon = 0.0001f) {
        if (is_quat_equal(a, b, epsilon)) {
            return testing::AssertionSuccess();
        } else {
            for (int i = 0; i < 4; i++) {
                if (!glm::epsilonEqual(a[i], b[i], epsilon)) {
                    return testing::AssertionFailure()
                            << a_expr << " and " << b_expr << " are not equal. "
                            << "\n   " << a_expr << "[" << i << "] = " << a[i] << " != " << b[i] << " = " << b_expr
                            << "[" << i << "]. "
                            << "Epsilon = " << epsilon_expr << " = " << epsilon << ".\n"
                            << "\t |" << a_expr << "\t | " << b_expr << "\n"
                            << "w\t | " << a.w << "\t | " << b.w << "\n"
                            << "x\t | " << a.x << "\t | " << b.x << "\n"
                            << "y\t | " << a.y << "\t | " << b.y << "\n"
                            << "z\t | " << a.z << "\t | " << b.z << "\n";


                }
            }
            return testing::AssertionFailure()
                    << a_expr << " and " << b_expr << " are not equal. "
                    << "Epsilon = " << epsilon_expr << " = " << epsilon << ".";
        }
    }

} // entre_portais

#endif //ENTREPORTAIS_TEST_UTILS_HPP
