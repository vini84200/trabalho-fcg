#include "entrePortaisEngine/Logger.hpp"

namespace entre_portais {
    std::shared_ptr<spdlog::sinks::sink> Logger::console_sink_ = nullptr;
    std::shared_ptr<spdlog::sinks::sink> Logger::file_sink_ = nullptr;
    std::shared_ptr<spdlog::logger> Logger::default_logger_ = nullptr;
    std::shared_ptr<spdlog::logger> Logger::console_logger_ = nullptr;
    std::shared_ptr<spdlog::logger> Logger::error_logger_ = nullptr;

    bool Logger::initialized_ = false;

    /** @deprecated */
    void log(const char *message) {
#ifdef DEBUG
        spdlog::warn("DEPRECATION WARNING: Use spdlog::info() instead of entre_portais::log()");
        spdlog::trace(message);
#endif
    }

    /** @deprecated */
    void log(const char *message, int value) {
#ifdef DEBUG
        spdlog::warn("DEPRECATION WARNING: Use spdlog::info() instead of entre_portais::log()");
        spdlog::trace("{} {} DEPRECATE", message, value);
#endif
    }

    void Logger::initialize() {
        if (initialized_) {
            return;
        }
        try {
            spdlog::cfg::load_env_levels();
            console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink_->set_level(spdlog::level::trace);

            file_sink_ = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/entrePortais.log", true);
            file_sink_->set_level(spdlog::level::trace);
            default_logger_ = std::make_shared<spdlog::logger>(spdlog::logger("default", {console_sink_, file_sink_}));
            default_logger_->set_level(spdlog::level::trace);
            default_logger_->flush_on(spdlog::level::warn);
            default_logger_->flush_on(spdlog::level::err);
            // TODO: Criar console logger
            // TODO: Criar error logger

            spdlog::flush_every(std::chrono::seconds(3));
            spdlog::set_default_logger(Logger::default_logger_);
        }
        catch (const spdlog::spdlog_ex &ex) {
            printf("Log initialization failed: %s \n", ex.what());
        }
        spdlog::info("Logger initialized");
        initialized_ = true;
    }

    Logger::Logger(const char *name) {
        if (!initialized_) {
            initialize();
        }
        logger_ = std::make_shared<spdlog::logger>(spdlog::logger(name, {console_sink_, file_sink_}));
        logger_->set_level(spdlog::level::trace);
        logger_->flush_on(spdlog::level::warn);
//        spdlog::register_logger(logger_);
    }
}  // namespace entre_portais