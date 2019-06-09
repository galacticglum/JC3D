/**
 * Author: Shon Verch
 * File Name: Logger.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/08/2019
 * Modified Date: 06/08/2019
 * Description: Message logging system.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Utilities/FlagEnum.h>

/**
 * @enum LoggerVerbosity Logger.h
 * @brief The different types of message verbosities.
 */
enum class LoggerVerbosity
{
	/**
	 * @brief Don't log the message.
	 */
	None = 1 << 0,

	/**
	 * @brief Log the message without any extra decorations (i.e. timestamp or category).
	 */
	Plain = 1 << 1,

	/**
	 * @brief Regular message.
	 */
	Info = 1 << 2,

	/**
	 * @brief Warning message.
	 */
	Warning = 1 << 3,

	/**
	 * @brief Error message
	 */
	Error = 1 << 4,
};

ENABLE_FLAG_ENUM(LoggerVerbosity)

/**
 * @enum LoggerDestination Logger.h
 * @brief The different types of destinations that the Logger can log to.
 */
enum class LoggerDestination
{
	/**
	 * @brief Don't log messages to anywhere.
	 */
	None = 1 << 0,

	/**
	 * @brief Log the message to standard output.
	 */
	Output = 1 << 1,

	/**
	 * @brief Log messages to file.
	 */
	File = 1 << 2,

	/**
	 * @brief Log messages to all destinations.
	 */
	All = File | Output
};

ENABLE_FLAG_ENUM(LoggerDestination)

/**
 * @struct LoggerMessageRecord Logger.h
 * @brief A record of a message that was logged.
 */
struct LoggerMessageRecord
{
	/**
	 * The message that was logged.
	 */
	const std::string Message;
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
	 * @param logFilePath The filepath of the log file (if LoggerDestination::File is enabled).
	 * @param messageFormat The message format.
	 * @param messageHeaderFormat The message header format.
	 */
	static void Initialize(const std::string& logFilePath = "", const std::string& messageFormat = "{header}: {message}",
		const std::string& messageHeaderFormat = "[{date}][{verbosity}] {category}");

	/**
	 * @brief The verbosities to display. Only a message with this verbosity (or higher) will be logged.
	 */
	static LoggerVerbosity VerbosityFilter;

	/**
	 * @brief The destination of log messages.
	 * @note A bitwise concatenation of all included logger destinations.
	 */
	static LoggerDestination Destination;

	/**
	 * @brief Line separator between messages.
	 */
	static std::string LineSeparator;

	/**
	 * @brief A global suffix for every message logged; appended to the end of every message.
	 */
	static std::string MessageSuffix;

	/**
	 * @brief The frequency at which lines should be separated.
	 */
	static int LineSeparatorMessageInterval;

	/**
	 * @brief When a message with this verbosity (or higher) is logged, the whole message buffer is flushed.
	 */
	static LoggerVerbosity FlushVerbosity;

	/**
	 * @brief The amount of messages the message buffer can store until it must flush itself.
	 */
	static int BufferMessageCapacity;

	/**
	 * @brief Log the @p message in the specified @p category with the @p messageVerbosity.
	 * @param category The category to log the @p message in.
	 * @param message The message to log.
	 * @param messageVerbosity The verbosity to log with.
	 * @param separateLineHere A boolean value indicating whether or not this message should be separated by a line.
	 */
	static void Log(const std::string& category, const std::string& message, LoggerVerbosity messageVerbosity = LoggerVerbosity::Info,
		bool separateLineHere = false);

	/**
	 * @brief Add a logger destination.
	 */
	static void AddDestination(LoggerDestination destination);

	/**
	 * @brief Remove a logger destination.
	 */
	static void RemoveDestination(LoggerDestination destination);

	/**
	 * @brief Indicates whether the specified @p destination is included in the Logger.
	 */
	static bool HasDestination(LoggerDestination destination);

	/**
	 * @brief Get the number of messages that have been logged since startup.
	 */
	static int GetMessageCount()
	{
		return m_RecordBuffer.size();
	}

	/**
	 * @brief Flush the file message buffer.
	 */
	static void FlushFileMessageBuffer();
private:
	/**
	 * @brief A mapping of LoggerVerbosity to string representation.
	 */
	static const std::unordered_map<LoggerVerbosity, std::string> s_VerbosityNames;

	/**
	 * @brief The category verbosity filter.
	 * @note If a category does not exist in this map, no filtering is applied for that category.
	 */
	static std::unordered_map<std::string, LoggerVerbosity> m_CategoryVerbosities;

	/**
	 * @brief A buffer containing all the logged messages that will be written to the log file, since the last flush.
	 */
	static std::vector<std::string> m_FileMessageBuffer;

	/**
	 * @brief A record of ALL the messages logged since startup.
	 */
	static std::vector<LoggerMessageRecord> m_RecordBuffer;

	/**
	 * @brief The path to the log file.
	 */
	static std::string m_LogFilePath;

	/**
	 * @brief The message format.
	 */
	static std::string m_MessageFormat;

	/**
	 * @brief The format of a message header.
	 */
	static std::string m_MessageHeaderFormat;

	/**
	 * @brief Gets the contents of the Logger::m_FileMessageBuffer as one large string.
	 */
	static std::string GetFileMessageBufferContents();

	/**
	 * @brief Gets the message header.
	 */
	static std::string GetMessageHeader(LoggerVerbosity verbosity, const std::string& category);

	/**
	 * @brief Get the length of the longest verbosity name.
	 */
	static std::size_t GetLongestVerbosityLength();

	/**
	 * @brief Get the default log file name.
	 */
	static std::string GetLogFileName();
};