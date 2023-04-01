#ifndef ENTREPORTAIS_UTILS_HPP
#define ENTREPORTAIS_UTILS_HPP

#include <barrier>

namespace utils {
    bool is_opengl_thread();


    inline float min(float a, float b) {
        if (a > b) {
            return b;
        } else {
            return a;
        }
    }

    std::string getTexturePath(const std::string& name);

}

#define MACHINE_EPSILON std::numeric_limits<float>::epsilon()
#define float_LARGEST std::numeric_limits<float>::max()

#endif //ENTREPORTAIS_UTILS_HPP
