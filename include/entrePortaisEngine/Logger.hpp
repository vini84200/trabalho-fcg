#ifndef ENTREPORTAIS_LOGGER_HPP
#define ENTREPORTAIS_LOGGER_HPP

#include <cstdio>
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
    printf("%s %f \n", message, static_cast<double>(value));
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
    printf("%s (%f, %f) \n", message, static_cast<double>(value.x), static_cast<double>(value.y));
#endif
}

void log(const char *message, const glm::vec3 &value) {
#ifdef DEBUG
    printf("%s (%f, %f, %f) \n", message, static_cast<double>(value.x), static_cast<double>(value.y),
           static_cast<double>(value.z));
#endif
}

void log(const char *message, const glm::vec4 &value) {
#ifdef DEBUG
    printf("%s (%f, %f, %f, %f) \n", message, static_cast<double>(value.x), static_cast<double>(value.y),
           static_cast<double>(value.z), static_cast<double>(value.w));
#endif
}

void log(const char *message, const glm::mat2 &value) {
#ifdef DEBUG
    printf("%s (\n%f, %f,\n %f, %f\n) \n", message, static_cast<double>(value[0][0]), static_cast<double>(value[0][1]),
           static_cast<double>(value[1][0]), static_cast<double>(value[1][1]));
#endif
}

#endif //ENTREPORTAIS_LOGGER_HPP
