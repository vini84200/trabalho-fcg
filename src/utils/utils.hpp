#ifndef ENTREPORTAIS_UTILS_HPP
#define ENTREPORTAIS_UTILS_HPP

namespace utils {
    float min(float a, float b) {
        if (a > b) {
            return b;
        } else {
            return a;
        }
    }
}

#endif //ENTREPORTAIS_UTILS_HPP
