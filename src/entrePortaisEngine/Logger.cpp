#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {

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

    void log(const char *message, double value) {
#ifdef DEBUG
        printf("%s %f \n", message, value);
#endif
    }
}