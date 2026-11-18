#include "log/logger.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <filesystem>
#include <memory>
#include <vector>

namespace xvm {
    void initLogger(bool logToFile, const std::string& logFile) {
        std::vector<spdlog::sink_ptr> sinks;

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_level(spdlog::level::trace);
        sinks.push_back(consoleSink);

        if (logToFile) {
            std::filesystem::path logPath(logFile);
            if (logPath.has_parent_path()) {
                std::filesystem::create_directories(logPath.parent_path());
            }

            constexpr std::size_t maxSize  = 5 * 1024 * 1024; // 5 MB max size, 3 rotated files
            constexpr std::size_t maxFiles = 3;
            auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFile, maxSize, maxFiles);
            fileSink->set_level(spdlog::level::trace);
            sinks.push_back(fileSink);
        }

        auto logger = std::make_shared<spdlog::logger>("xvm", sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        spdlog::set_default_logger(logger);
        spdlog::flush_on(spdlog::level::info);

        spdlog::info("Logger initialised (file logging: {})", logToFile ? "enabled" : "disabled");
    }
}