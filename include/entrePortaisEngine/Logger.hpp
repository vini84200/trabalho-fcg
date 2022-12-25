#include <cstdio>
#include <glm/glm.hpp>

#ifndef ENTREPORTAIS_LOGGER_HPP
#define ENTREPORTAIS_LOGGER_HPP
namespace entre_portais {
    void log(const char *message);

    void log(const char *message, int value);

    void log(const char *message, double value);
}
#endif //ENTREPORTAIS_LOGGER_HPP
