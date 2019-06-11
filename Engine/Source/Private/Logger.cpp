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
std::string Logger::m_DefaultMessageFormat;

void Logger::Initialize(const std::string& defaultMessageFormat, const bool force)
{
	if (s_IsInitialized && !force) return;

	m_DefaultMessageFormat = defaultMessageFormat;
}

void Logger::RegisterCategory(const std::string& name, const std::shared_ptr<spdlog::logger>& logger,
	const std::string& messageFormat)
{
	// Return if this category is already registered.
	if (s_Loggers.find(name) != s_Loggers.end()) return;

	s_Loggers[name] = logger;
	s_Loggers[name]->set_level(spdlog::level::trace);
	s_Loggers[name]->set_pattern(messageFormat);
}

const std::shared_ptr<spdlog::logger>& Logger::GetLogger()
{
	return GetLogger(GLOBAL_LOGGER_IDENTIFIER);
}

const std::shared_ptr<spdlog::logger>& Logger::GetLogger(const std::string& category)
{
	Initialize();
	if (s_Loggers.find(category) == s_Loggers.end())
	{
		const auto logger = spdlog::stdout_color_mt(category);
		RegisterCategory(category, logger);
	}

	return s_Loggers[category];
}

