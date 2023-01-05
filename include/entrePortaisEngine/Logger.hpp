#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/cfg/env.h"
#include "spdlog/async.h"
#include "spdlog/fmt/ostr.h"

#ifndef ENTREPORTAIS_LOGGER_HPP
#define ENTREPORTAIS_LOGGER_HPP

namespace entre_portais {
    class Logger {
    public:
        static void initialize();

        static std::shared_ptr<spdlog::logger> getConsoleLogger() {
            return console_logger_;
        }

        static std::shared_ptr<spdlog::logger> getDefaultLogger() {
            return default_logger_;
        }

        static std::shared_ptr<spdlog::logger> getErrorLogger() {
            return error_logger_;
        }

        Logger(const char *name);

        std::shared_ptr<spdlog::logger> getLogger() {
            return logger_;
        }


    private:
        std::shared_ptr<spdlog::logger> logger_;
        const char *name_;

        static bool initialized_;
        static std::shared_ptr<spdlog::sinks::sink> console_sink_;
        static std::shared_ptr<spdlog::sinks::sink> file_sink_;
        static std::shared_ptr<spdlog::logger> default_logger_;
        static std::shared_ptr<spdlog::logger> console_logger_;
        static std::shared_ptr<spdlog::logger> error_logger_;
    };

    void log(const char *message);

    void log(const char *message, int value);
}
#endif //ENTREPORTAIS_LOGGER_HPP
