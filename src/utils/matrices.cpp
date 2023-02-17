#include "utils/matrices.h"

glm::mat4 matrices::Matrix(
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
    float m33)
{
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

glm::mat4 matrices::Matrix_Identity()
{
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

glm::mat4 matrices::Matrix_Translate(float tx, float ty, float tz)
{
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

glm::mat4 matrices::Matrix_Scale(float sx, float sy, float sz)
{
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

glm::mat4 matrices::Matrix_Rotate_X(float angle)
{
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

glm::mat4 matrices::Matrix_Rotate_Y(float angle)
{
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

glm::mat4 matrices::Matrix_Rotate_Z(float angle)
{
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

float matrices::norm(glm::vec4 v)
{
  float vx = v.x;
  float vy = v.y;
  float vz = v.z;

  return sqrt(vx * vx + vy * vy + vz * vz);
}

glm::mat4 matrices::Matrix_Rotate(float angle, glm::vec4 axis)
{
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

glm::vec4 matrices::crossproduct(glm::vec4 u, glm::vec4 v)
{
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

float matrices::dotproduct(glm::vec4 u, glm::vec4 v)
{
  float ux = u.x;
  float uy = u.y;
  float uz = u.z;
  float uw = u.w;
  float vx = v.x;
  float vy = v.y;
  float v3 = v.z;
  float vw = v.w;

  if (uw != 0.0f || vw != 0.0f)
  {
    fprintf(stderr, "ERROR: Produto escalar não definido para pontos.\n");
    std::exit(EXIT_FAILURE);
  }

  return ux * vx + uy * vy + uz * v3 + uw * vw;
}

glm::mat4 matrices::Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector)
{
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

glm::mat4 matrices::Matrix_Orthographic(float l, float r, float b, float t, float n, float f)
{
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

glm::mat4 matrices::Matrix_Perspective(float field_of_view, float aspect, float n, float f)
{
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

void matrices::PrintMatrix(glm::mat4 M)
{
  printf("\n");
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0]);
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1]);
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2]);
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3]);
}

void matrices::PrintVector(glm::vec4 v)
{
  printf("\n");
  printf("[ %+0.2f ]\n", v[0]);
  printf("[ %+0.2f ]\n", v[1]);
  printf("[ %+0.2f ]\n", v[2]);
  printf("[ %+0.2f ]\n", v[3]);
}

void matrices::PrintMatrixVectorProduct(glm::mat4 M, glm::vec4 v)
{
  auto r = M * v;
  printf("\n");
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][0], M[1][0], M[2][0], M[3][0], v[0], r[0]);
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ] = [ %+0.2f ]\n", M[0][1], M[1][1], M[2][1], M[3][1], v[1], r[1]);
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][2], M[1][2], M[2][2], M[3][2], v[2], r[2]);
  printf("[ %+0.2f  %+0.2f  %+0.2f  %+0.2f ][ %+0.2f ]   [ %+0.2f ]\n", M[0][3], M[1][3], M[2][3], M[3][3], v[3], r[3]);
}

void matrices::PrintMatrixVectorProductDivW(glm::mat4 M, glm::vec4 v)
{
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
glm::vec4 matrices::Vector_From_Euler(float size, float alfa, float beta, float gama)
{
  glm::vec4 x = glm::vec4(size, 0, 0, 0);
  float ca = cos(alfa), cb = cos(beta), cg = cos(gama);
  float sa = sin(alfa), sb = sin(beta), sg = sin(gama);
  glm::mat4 eulerMatrix = Matrix(
      ca*cg-sa*cb*sg, sa*cg+ca*cb*sg, sb*sg, 0,
      -ca*sg-sa*cb*cg, -sa*sg+ca*cb*cg, sb*cg, 0,
      sa*sb, -ca*sb, cb, 0,
      0, 0, 0, 1
      );

  return eulerMatrix * x;
}
