// FONTE: LABORATÓRIO 2 - COMPUTAÇÃO GRÁFICA - 2022/2
#ifndef _MATRICES_H
#define _MATRICES_H

#include <cstdio>
#include <cstdlib>

#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/detail/type_quat.hpp"

namespace matrices {

// Esta função Matrix() auxilia na criação de matrizes usando a biblioteca GLM.
// Note que em OpenGL (e GLM) as matrizes são definidas como "column-major",
// onde os elementos da matriz são armazenadas percorrendo as COLUNAS da mesma.
// Por exemplo, seja
//
//       [a b c]
//   M = [d e f]
//       [g h i]
//
// uma matriz 3x3. Em memória, na representação "column-major" de OpenGL, essa
// matriz é representada pelo seguinte array:
//
//   M[] = {  a,d,g,    b,e,h,    c,f,i  };
//              ^         ^         ^
//              |         |         |
//           coluna 1  coluna 2  coluna 3
//
// Para conseguirmos definir matrizes através de suas LINHAS, a função Matrix()
// computa a transposta usando os elementos passados por parâmetros.
    constexpr glm::mat4 Matrix(
            float m00,
            float m01,
            float m02,
            float m03,
            float m10,
            float m11,
            float m12,
            float m13,
            float m20,
            float m21,
            float m22,
            float m23,
            float m30,
            float m31,
            float m32,
            float m33) {
        return glm::mat4(
                m00,
                m10,
                m20,
                m30,  // COLUNA 1
                m01,
                m11,
                m21,
                m31,  // COLUNA 2
                m02,
                m12,
                m22,
                m32,  // COLUNA 3
                m03,
                m13,
                m23,
                m33  // COLUNA 4
        );
    }

    constexpr glm::mat4 Matrix_Identity() {
        return Matrix(
                1.0f,
                0.0f,
                0.0f,
                0.0f,  // LINHA 1
                0.0f,
                1.0f,
                0.0f,
                0.0f,  // LINHA 2
                0.0f,
                0.0f,
                1.0f,
                0.0f,  // LINHA 3
                0.0f,
                0.0f,
                0.0f,
                1.0f  // LINHA 4
        );
    }

    constexpr glm::mat4 Matrix_Translate(float tx, float ty, float tz) {
        return Matrix(
                1.0f,
                0.0f,
                0.0f,
                tx,  // LINHA 1
                0.0f,
                1.0f,
                0.0f,
                ty,  // LINHA 2
                0.0f,
                0.0f,
                1.0f,
                tz,  // LINHA 3
                0.0f,
                0.0f,
                0.0f,
                1.0f  // LINHA 4
        );
    }

    constexpr glm::mat4 Matrix_Scale(float sx, float sy, float sz) {
        return Matrix(
                sx,
                0.0f,
                0.0f,
                0.0f,  // LINHA 1
                0.0f,
                sy,
                0.0f,
                0.0f,  // LINHA 2
                0.0f,
                0.0f,
                sz,
                0.0f,  // LINHA 3
                0.0f,
                0.0f,
                0.0f,
                1.0f  // LINHA 4
        );
    }

    constexpr glm::mat4 Matrix_Rotate_X(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        return Matrix(
                1.0f,
                0.0f,
                0.0f,
                0.0f,  // LINHA 1
                0.0f,
                c,
                -s,
                0.0f,  // LINHA 2
                0.0f,
                s,
                c,
                0.0f,  // LINHA 3
                0.0f,
                0.0f,
                0.0f,
                1.0f  // LINHA 4
        );
    }

    constexpr glm::mat4 Matrix_Rotate_Y(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        return Matrix(
                c,
                0.0f,
                s,
                0.0f,  // LINHA 1
                0.0f,
                1.0f,
                0.0f,
                0.0f,  // LINHA 2
                -s,
                0.0f,
                c,
                0.0f,  // LINHA 3
                0.0f,
                0.0f,
                0.0f,
                1.0f  // LINHA 4
        );
    }

    constexpr glm::mat4 Matrix_Rotate_Z(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        return Matrix(
                c,
                -s,
                0.0f,
                0.0f,  // LINHA 1
                s,
                c,
                0.0f,
                0.0f,  // LINHA 2
                0.0f,
                0.0f,
                1.0f,
                0.0f,  // LINHA 3
                0.0f,
                0.0f,
                0.0f,
                1.0f  // LINHA 4
        );
    }

    constexpr float norm(glm::vec4 v) {
        float vx = v.x;
        float vy = v.y;
        float vz = v.z;

        return sqrt(vx * vx + vy * vy + vz * vz);
    }

    constexpr glm::mat4 Matrix_Rotate(float angle, glm::vec4 axis) {
        float c = cos(angle);
        float s = sin(angle);
        float d = 1 - c;

        glm::vec4 v = axis / norm(axis);

        float vx = v.x;
        float vy = v.y;
        float vz = v.z;

        return Matrix(
                vx * vx * d + c,
                vx * vy * d - vz * s,
                vx * vz * d + vy * s,
                0.0f,
                vx * vy * d + vz * s,
                vy * vy * d + c,
                vy * vz * d - vx * s,
                0.0f,
                vx * vz * d - vy * s,
                vy * vz * d + vx * s,
                vz * vz * d + c,
                0.0f,
                0.0f,
                0.0f,
                0.0f,
                1.0f);
    }

    constexpr glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v) {
        float ux = u.x;
        float uy = u.y;
        float uz = u.z;
        float vx = v.x;
        float vy = v.y;
        float vz = v.z;

        return glm::vec4(
                uy * vz - uz * vy,  // Primeiro coeficiente
                uz * vx - ux * vz,  // Segundo coeficiente
                ux * vy - uy * vx,  // Terceiro coeficiente
                0.0f                // w = 0 para vetores.
        );
    }

    constexpr float dotproduct(glm::vec4 u, glm::vec4 v) {
        float ux = u.x;
        float uy = u.y;
        float uz = u.z;
        float uw = u.w;
        float vx = v.x;
        float vy = v.y;
        float v3 = v.z;
        float vw = v.w;

        if (uw != 0.0f || vw != 0.0f) {
            fprintf(stderr, "ERROR: Produto escalar não definido para pontos.\n");
            std::exit(EXIT_FAILURE);
        }

        return ux * vx + uy * vy + uz * v3 + uw * vw;
    }

    constexpr glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector) {
        glm::vec4 w = -view_vector;
        glm::vec4 u = crossproduct(up_vector, w);

        // Normalizamos os vetores u e w
        w = w / norm(w);
        u = u / norm(u);

        glm::vec4 v = crossproduct(w, u);

        glm::vec4 origin_o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        float ux = u.x;
        float uy = u.y;
        float uz = u.z;
        float vx = v.x;
        float vy = v.y;
        float vz = v.z;
        float wx = w.x;
        float wy = w.y;
        float wz = w.z;

        return Matrix(
                ux,
                uy,
                uz,
                -dotproduct(u, position_c - origin_o),
                vx,
                vy,
                vz,
                -dotproduct(v, position_c - origin_o),
                wx,
                wy,
                wz,
                -dotproduct(w, position_c - origin_o),
                0.0f,
                0.0f,
                0.0f,
                1.0f);
    }

    constexpr glm::mat4 Matrix_Orthographic(float l, float r, float b, float t, float n, float f) {
        glm::mat4 M = Matrix(
                2 / (r - l),
                0,
                0,
                -(r + l) / (r - l),
                0,
                2 / (t - b),
                0,
                -(t + b) / (t - b),
                0,
                0,
                2 / (f - n),
                -(f + n) / (f - n),
                0,
                0,
                0,
                1);

        return M;
    }

// Matriz de projeção perspectiva
    glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f);

// Função que imprime uma matriz M no terminal
    void PrintMatrix(glm::mat4 M);

// Função que imprime um vetor v no terminal
    void PrintVector(glm::vec4 v);

// Função que imprime o produto de uma matriz por um vetor no terminal
    void PrintMatrixVectorProduct(glm::mat4 M, glm::vec4 v);

// Função que imprime o produto de uma matriz por um vetor, junto com divisão
// por w, no terminal.
    void PrintMatrixVectorProductDivW(glm::mat4 M, glm::vec4 v);

    glm::vec4 PositionFromMatrix(glm::mat4 M);

    glm::quat RotationFromMatrix(glm::mat4 M);

    glm::vec4 ScaleFromMatrix(glm::mat4 M);

    glm::mat4 ScalingMatrixFromMatrix(glm::mat4 mat);

    constexpr glm::mat4 RotationFromQuat(glm::quat rotation) {
        const auto q0 = rotation.w;
        auto q1 = rotation.z;
        auto q2 = rotation.y;
        auto q3 = rotation.x;

        return matrices::Matrix(
                1 - 2 * q2 * q2 - 2 * q3 * q3, 2 * q1 * q2 - 2 * q3 * q0, 2 * q1 * q3 + 2 * q2 * q0, 0,
                2 * q1 * q2 + 2 * q0 * q3, 1 - 2 * q1 * q1 - 2 * q3 * q3, 2 * q2 * q3 - 2 * q1 * q0, 0,
                2 * q1 * q3 - 2 * q0 * q2, 2 * q2 * q3 + 2 * q1 * q0, 1 - 2 * q1 * q1 - 2 * q2 * q2, 0,
                0, 0, 0, 1
        );
    }

    glm::vec4 Vector_From_Euler(float size, float alfa, float beta, float gama);

}

#endif // _MATRICES_H
// vim: set spell spelllang=pt_br :
