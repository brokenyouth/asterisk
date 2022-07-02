#include "Logger.h"
#include <vector>

std::shared_ptr<spdlog::logger> Logger::sCoreLogger;
std::shared_ptr<spdlog::logger> Logger::sClientLogger;

Logger::Logger()
{

}

Logger::~Logger()
{

}

void Logger::Initialize()
{
    std::vector<spdlog::sink_ptr> LogSinks;
    LogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    LogSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Asterisk_LOG.log", true));

    LogSinks[0]->set_pattern("%^[%T] %n: %v%$");
    LogSinks[1]->set_pattern("[%T] [%l] %n: %v");

    sCoreLogger = std::make_shared<spdlog::logger>("ENGINE", std::begin(LogSinks), std::end(LogSinks));
    spdlog::register_logger(sCoreLogger);
    sCoreLogger->set_level(spdlog::level::trace);
    sCoreLogger->flush_on(spdlog::level::trace);

    sClientLogger = std::make_shared<spdlog::logger>("GAME", std::begin(LogSinks), std::end(LogSinks));
    spdlog::register_logger(sClientLogger);
    sClientLogger->set_level(spdlog::level::trace);
    sClientLogger->flush_on(spdlog::level::trace);
}