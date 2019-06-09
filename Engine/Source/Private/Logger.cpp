#include <Logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

const std::unordered_map<LoggerVerbosity, std::string> Logger::s_VerbosityNames = 
{
	{ LoggerVerbosity::Off, "off" },
	{ LoggerVerbosity::Trace, "trace" },
	{ LoggerVerbosity::Info, "info" },
	{ LoggerVerbosity::Warning, "warning" },
	{ LoggerVerbosity::Error, "error" },
	{ LoggerVerbosity::Fatal, "fatal" },

};

std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::s_Loggers;
bool Logger::s_IsInitialized = false;

void Logger::Initialize(const bool force)
{
	if (s_IsInitialized && !force) return;

	spdlog::set_pattern("%^[%T] %n: %v%$");

	// Create a global logger.
	s_Loggers[GLOBAL_LOGGER_IDENTIFIER] = spdlog::stdout_color_mt("GLOBAL");
	s_Loggers[GLOBAL_LOGGER_IDENTIFIER]->set_level(spdlog::level::trace);
}

void Logger::RegisterCategory(const std::string& name, const std::shared_ptr<spdlog::logger>& logger)
{
	s_Loggers[name] = logger;
}

const std::shared_ptr<spdlog::logger>& Logger::GetLogger()
{
	Initialize();
	return GetLogger(GLOBAL_LOGGER_IDENTIFIER);
}

const std::shared_ptr<spdlog::logger>& Logger::GetLogger(const std::string& category)
{
	if (s_Loggers.find(category) == s_Loggers.end())
	{
		const auto logger = spdlog::stdout_color_mt(category);
		RegisterCategory(category, logger);
	}

	return s_Loggers[category];
}

