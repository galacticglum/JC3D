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

void Logger::Initialize(const std::string& defaultMessageFormat, const bool force)
{
	if (s_IsInitialized && !force) return;

	m_DefaultMessageFormat = defaultMessageFormat;

	// Create a global logger.
	RegisterCategory(GLOBAL_LOGGER_IDENTIFIER, spdlog::stdout_color_mt("GLOBAL"));
}

void Logger::RegisterCategory(const std::string& name, const std::shared_ptr<spdlog::logger>& logger,
	const std::string& messageFormat)
{
	s_Loggers[name] = logger;
	s_Loggers[name]->set_level(spdlog::level::trace);
	s_Loggers[name]->set_pattern(messageFormat);
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

