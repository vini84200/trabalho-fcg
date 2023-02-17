#ifndef ENTREPORTAIS_COMPATIBILITY_HPP
#define ENTREPORTAIS_COMPATIBILITY_HPP
#include <chrono>
#include <thread>

namespace entre_portais {

    inline void sleep_for_millis(long int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    };

    inline void sleep_for_nanoseconds(long int ns) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(ns));
    }

}
#endif //ENTREPORTAIS_COMPATIBILITY_HPP