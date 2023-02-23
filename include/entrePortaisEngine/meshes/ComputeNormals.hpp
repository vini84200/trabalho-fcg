#ifndef ENTREPORTAIS_COMPUTENORMALS_HPP
#define ENTREPORTAIS_COMPUTENORMALS_HPP

void computeAllSmoothingNormals(tinyobj::attrib_t &attrib, std::vector<tinyobj::shape_t> &shapes);

void computeSmoothingShape(tinyobj::attrib_t &inattrib, tinyobj::shape_t &inshape,
                           std::vector<std::pair<unsigned int, unsigned int>> &sortedids,
                           unsigned int idbegin, unsigned int idend,
                           std::vector<tinyobj::shape_t> &outshapes,
                           tinyobj::attrib_t &outattrib);

void computeSmoothingShapes(tinyobj::attrib_t &inattrib,
                            std::vector<tinyobj::shape_t> &inshapes,
                            std::vector<tinyobj::shape_t> &outshapes,
                            tinyobj::attrib_t &outattrib);


#endif //ENTREPORTAIS_COMPUTENORMALS_HPP
