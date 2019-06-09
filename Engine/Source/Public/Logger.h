/**
 * Author: Shon Verch
 * File Name: Logger.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/08/2019
 * Modified Date: 06/09/2019
 * Description: Message logging system.
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <spdlog/spdlog.h>

#define GLOBAL_LOGGER_IDENTIFIER "Global"

/**
 * @enum LoggerVerbosity LoggerVerbosity.h
 * @brief The logger levels.
 */
enum class LoggerVerbosity
{
	Off,
	Trace,
	Info,
	Warning,
	Error,
	Fatal
};

/**
 * @class Logger Logger.h
 * @brief Message logging system.
 */
class Logger
{
public:
	/**
	 * @brief Initialize the logger.
	 * @note This method is automatically called by the Logger before
	 *		 any logging operations; however, the Logger can also be
	 *		 manually initialized.
	 *	@param force A boolean value indicating whether or not the Logger
	 *			     should be initialized even if it has already been initialized.
	 *			     This is useful when Logger settings need to be changed during runtime.
	 */
	static void Initialize(bool force = false);

	/**
	 * @brief Log in the specified @p category.
	 */
	template<typename... Args>
	static void Log(const std::string& category, const LoggerVerbosity verbosity, const Args&... args)
	{
		InternalLog(category, verbosity, args...);
	}

	/**
	 * @brief Log in the specified @p category.
	 */
	template<typename... Args>
	static void Log(const LoggerVerbosity verbosity, const Args&... args)
	{
		InternalLog(GLOBAL_LOGGER_IDENTIFIER, verbosity, args...);
	}

	/**
	 * @brief Adds a category with the specified logger.
	 */
	static void RegisterCategory(const std::string& name, const std::shared_ptr<spdlog::logger>& logger);

	/**
	 * @brief Gets the global logger instance.
	 */
	static const std::shared_ptr<spdlog::logger>& GetLogger();

	/**
	 * @brief Get a logger instance for the specified @p category.
	 * @param category The category of the logger.
	 */
	static const std::shared_ptr<spdlog::logger>& GetLogger(const std::string& category);
private:
	template<typename... Args>
	static void InternalLog(const std::string& category, const LoggerVerbosity verbosity, const Args&... args)
	{
		const std::string verbosityName = s_VerbosityNames.find(verbosity)->second;
		auto level = spdlog::level::from_str(verbosityName);
		GetLogger(category)->log(level, args...);
	}

	static const std::unordered_map<LoggerVerbosity, std::string> s_VerbosityNames;
	static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_Loggers;
	static bool s_IsInitialized;
};