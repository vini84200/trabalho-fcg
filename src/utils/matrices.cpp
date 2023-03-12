#include "utils/matrices.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtx/quaternion.hpp"


glm::mat4 matrices::Matrix_Perspective(float field_of_view, float aspect, float n, float f) {
    float t = fabs(n) * tanf(field_of_view / 2.0f);
    float b = -t;
    float r = t * aspect;
    float l = -r;

    glm::mat4 P = Matrix(n, 0, 0, 0, 0, n, 0, 0, 0, 0, n + f, -n * f, 0, 0, 1, 0);

    // A matriz M é a mesma computada acima em Matrix_Orthographic().
    glm::mat4 M = Matrix_Orthographic(l, r, b, t, n, f);

    // Note que as matrizes M*P e -M*P fazem exatamente a mesma projeção
    // perspectiva, já que o sinal de negativo não irá afetar o resultado
    // devido à divisão por w. Por exemplo, seja q = [qx,qy,qz,1] um ponto:
    //
    //      M*P*q = [ qx', qy', qz', w ]
    //   =(div w)=> [ qx'/w, qy'/w, qz'/w, 1 ]   Eq. (*)
    //
    // agora com o sinal de negativo:
    //
    //     -M*P*q = [ -qx', -qy', -qz', -w ]
    //   =(div w)=> [ -qx'/-w, -qy'/-w, -qz'/-w, -w/-w ]
    //            = [ qx'/w, qy'/w, qz'/w, 1 ]   Eq. (**)
    //
    // Note que o ponto final, após divisão por w, é igual: Eq. (*) == Eq. (**).
    //
    // Então, por que utilizamos -M*P ao invés de M*P? Pois a especificação de
    // OpenGL define que os pontos fora do cubo unitário NDC deverão ser
    // descartados já que não irão aparecer na tela. O teste que define se um ponto
    // q está dentro do cubo unitário NDC pode ser expresso como:
    //
    //      -1 <= qx'/w <= 1   &&  -1 <= qy'/w <= 1   &&  -1 <= qz'/w <= 1
    //
    // ou, de maneira equivalente SE w > 0, a placa de vídeo faz o seguinte teste
    // ANTES da divisão por w:
    //
    //      -w <= qx' <= w   &&  -w <= qy' <= w   &&  -w <= qz' <= w
    //
    // Note que o teste acima economiza uma divisão por w caso o ponto seja
    // descartado (quando esteja fora de NDC), entretanto, este último teste só
    // é equivalente ao primeiro teste SE E SOMENTE SE w > 0 (isto é, se w for
    // positivo). Como este último teste é o que a placa de vídeo (GPU) irá fazer,
    // precisamos utilizar a matriz -M*P para projeção perspectiva, de forma que
    // w seja positivo.
    //
    return -M * P;
}

void matrices::PrintMatrix(glm::mat4 M) {
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3]);
}

void matrices::PrintVector(glm::vec4 v) {
    printf("\n");
    printf("[ %+0.2f ]\n", v[0]);
    printf("[ %+0.2f ]\n", v[1]);
    printf("[ %+0.2f ]\n", v[2]);
    printf("[ %+0.2f ]\n", v[3]);
}

void matrices::PrintMatrixVectorProduct(glm::mat4 M, glm::vec4 v) {
    auto r = M * v;
    printf("\n");
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0], v[0],
           r[0]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ] = [ %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1], v[1],
           r[1]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2], v[2],
           r[2]);
    printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3], v[3],
           r[3]);
}

void matrices::PrintMatrixVectorProductDivW(glm::mat4 M, glm::vec4 v) {
    auto r = M * v;
    auto w = r[3];
    printf("\n");
    printf(
            "[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]            [ %+0.2f ]\n",
            M[0][0],
            M[1][0],
            M[2][0],
            M[3][0],
            v[0],
            r[0],
            r[0] / w);
    printf(
            "[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ] = [ %+0.2f ] =(div w)=> [ %+0.2f ]\n",
            M[0][1],
            M[1][1],
            M[2][1],
            M[3][1],
            v[1],
            r[1],
            r[1] / w);
    printf(
            "[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]            [ %+0.2f ]\n",
            M[0][2],
            M[1][2],
            M[2][2],
            M[3][2],
            v[2],
            r[2],
            r[2] / w);
    printf(
            "[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]            [ %+0.2f ]\n",
            M[0][3],
            M[1][3],
            M[2][3],
            M[3][3],
            v[3],
            r[3],
            r[3] / w);
}

glm::vec4 matrices::Vector_From_Euler(float size, float alfa, float beta, float gama) {
    glm::vec4 x = glm::vec4(size, 0, 0, 0);
    float ca = cos(alfa), cb = cos(beta), cg = cos(gama);
    float sa = sin(alfa), sb = sin(beta), sg = sin(gama);
    glm::mat4 eulerMatrix = Matrix(
            ca * cg - sa * cb * sg, sa * cg + ca * cb * sg, sb * sg, 0,
            -ca * sg - sa * cb * cg, -sa * sg + ca * cb * cg, sb * cg, 0,
            sa * sb, -ca * sb, cb, 0,
            0, 0, 0, 1
    );

    return eulerMatrix * x;
}


glm::vec4 matrices::PositionFromMatrix(glm::mat4 M) {
    return glm::vec4(M[3][0], M[3][1], M[3][2], M[3][3]);
}

glm::vec4 matrices::ScaleFromMatrix(glm::mat4 M) {
    return glm::vec4(
            glm::length(glm::vec3(M[0])),
            glm::length(glm::vec3(M[1])),
            glm::length(glm::vec3(M[2])),
            0
    );
}

glm::mat4 matrices::ScalingMatrixFromMatrix(glm::mat4 mat) {
    auto scale = ScaleFromMatrix(mat);
    return matrices::Matrix(
            scale.x, 0, 0, 0,
            0, scale.y, 0, 0,
            0, 0, scale.z, 0,
            0, 0, 0, 1
    );
}

glm::quat matrices::RotationFromMatrix(glm::mat4 M) {
    auto scale = ScaleFromMatrix(M);
    auto scaleMatrix = matrices::Matrix(
            1 / scale.x, 0, 0, 0,
            0, 1 / scale.y, 0, 0,
            0, 0, 1 / scale.z, 0,
            0, 0, 0, 1
    );
    auto rotationMatrix = M * scaleMatrix;
    return glm::quat_cast(rotationMatrix);
}

glm::mat4 matrices::RotationFromQuat(glm::quat rotation) {
    const glm::quat q = glm::normalize(rotation);
    const auto q0 = q.w;
    auto q1 = q.z;
    auto q2 = q.y;
    auto q3 = q.x;

    return matrices::Matrix(
            1 - 2 * q2 * q2 - 2 * q3 * q3, 2 * q1 * q2 - 2 * q3 * q0, 2 * q1 * q3 + 2 * q2 * q0, 0,
            2 * q1 * q2 + 2 * q0 * q3, 1 - 2 * q1 * q1 - 2 * q3 * q3, 2 * q2 * q3 - 2 * q1 * q0, 0,
            2 * q1 * q3 - 2 * q0 * q2, 2 * q2 * q3 + 2 * q1 * q0, 1 - 2 * q1 * q1 - 2 * q2 * q2, 0,
            0, 0, 0, 1
    );

}

const glm::mat3 matrices::inertiaTensorSphere(float m, float r) {
    float I = 2.0f / 5.0f * m * r * r;
    return glm::mat3(I, 0, 0, 0, I, 0, 0, 0, I);
}

glm::mat3 matrices::inertiaTensorBox(float mass, glm::vec3 size) {
    float x2 = size.x * size.x;
    float y2 = size.y * size.y;
    float z2 = size.z * size.z;

    float Ix = 1.0f / 12.0f * mass * (y2 + z2);
    float Iy = 1.0f / 12.0f * mass * (x2 + z2);
    float Iz = 1.0f / 12.0f * mass * (x2 + y2);

    return glm::mat3(Ix, 0, 0, 0, Iy, 0, 0, 0, Iz);
}
