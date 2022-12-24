#ifndef ENTREPORTAIS_LOGGER_HPP
#define ENTREPORTAIS_LOGGER_HPP

#include <iostream>
#include <glm/glm.hpp>

void log(const char *message) {
#ifdef DEBUG
    printf("%s \n", message);
#endif
}

void log(const char *message, int value) {
#ifdef DEBUG
    printf("%s %d \n", message, value);
#endif
}

void log(const char *message, float value) {
#ifdef DEBUG
    printf("%s %f \n", message, value);
#endif
}

void log(const char *message, double value) {
#ifdef DEBUG
    printf("%s %f \n", message, value);
#endif
}

void log(const char *message, const char *value) {
#ifdef DEBUG
    printf("%s %s \n", message, value);
#endif
}

void log(const char *message, bool value) {
#ifdef DEBUG
    printf("%s %s \n", message, value ? "true" : "false");
#endif
}


void log(const char *message, const glm::vec2 &value) {
#ifdef DEBUG
    printf("%s (%f, %f) \n", message, value.x, value.y);
#endif
}

#endif //ENTREPORTAIS_LOGGER_HPP
