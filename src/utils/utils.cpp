#include <barrier>
#include "utils/utils.hpp"

namespace utils {
    const std::thread::id opengl_thread = std::this_thread::get_id();

    bool is_opengl_thread() {
        return std::this_thread::get_id() == opengl_thread;
    }

    std::string getTexturePath(const std::string &name) {
        return "assets/textures/" + name;
    }

}