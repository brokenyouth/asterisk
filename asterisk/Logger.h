#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger
{
private:
	static std::shared_ptr<spdlog::logger> sCoreLogger;
	static std::shared_ptr<spdlog::logger> sClientLogger;
public:
	Logger();
	~Logger();
public:
	static void Initialize();
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

};

// Core Log Macros
#define LOG_CORE_ERROR(...)	::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_WARN(...)		::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_INFO(...)		::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_TRACE(...)	::Logger::GetCoreLogger()->trace(__VA_ARGS__)

// Client Log Macros
#define LOG_ERROR(...)			::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)			::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)			::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)			::Logger::GetCoreLogger()->trace(__VA_ARGS__)