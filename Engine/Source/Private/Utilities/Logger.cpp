#include <Utilities/Logger.h>
#include <System/File.h>

#include <sstream>
#include <iomanip>
#include <iostream>
#include <fmt/format.h>
#include <ctime>
#include <chrono>

const std::unordered_map<LoggerVerbosity, std::string> Logger::s_VerbosityNames =
{
	{ LoggerVerbosity::None, "None" },
	{ LoggerVerbosity::Plain, "Plain" },
	{ LoggerVerbosity::Info, "Info" },
	{ LoggerVerbosity::Warning, "Warning" },
	{ LoggerVerbosity::Error, "Error" }
};

int Logger::LineSeparatorMessageInterval = -1;
LoggerVerbosity Logger::FlushVerbosity = LoggerVerbosity::Error;
int Logger::BufferMessageCapacity = 100;
LoggerVerbosity Logger::VerbosityFilter = LoggerVerbosity::Plain;

void Logger::Initialize(const std::string& logFilePath, const std::string& messageFormat,
	const std::string& messageHeaderFormat)
{
	m_LogFilePath = logFilePath.empty() ? GetLogFileName() : logFilePath;

	// Delete the log file if it exists.
	File::Delete(m_LogFilePath);
}

void Logger::Log(const std::string& category, const std::string& message, LoggerVerbosity messageVerbosity,
                 const bool separateLineHere)
{
	// The none verbosity turns off logging.
	if (messageVerbosity == LoggerVerbosity::None) return;
	if (VerbosityFilter > messageVerbosity) return;

	// Check if the category we are trying to log to allows the specified verbosity
	if (m_CategoryVerbosities.find(category) != m_CategoryVerbosities.end() && 
		m_CategoryVerbosities[category] > messageVerbosity) return;

	const std::string header = GetMessageHeader(messageVerbosity, category);
	std::string output = fmt::format(m_MessageFormat, fmt::arg("header", header), fmt::arg("message", message)) + MessageSuffix;

	std::string messageSeparator;
	const bool shouldSeparateLine = separateLineHere || LineSeparatorMessageInterval > 0 && GetMessageCount() % LineSeparatorMessageInterval == 0;
	if (!LineSeparator.empty() && shouldSeparateLine)
	{
		for  (int i =  0; i < output.size(); ++i)
		{
			messageSeparator += LineSeparator;
		}
	}

	// Log to the file
	if (HasDestination(LoggerDestination::File))
	{
		m_FileMessageBuffer.push_back(output);
		if(!messageSeparator.empty())
		{
			m_FileMessageBuffer.push_back(messageSeparator);
		}
	}

	// Log to the console
	if (HasDestination(LoggerDestination::Output))
	{
		if (messageVerbosity == LoggerVerbosity::Plain)
		{
			std::cout << message << "\n";
		}
		else
		{
			std::cout << output << "\n";
		}

		if(!messageSeparator.empty())
		{
			std::cout << messageSeparator << "\n";
		}
	}

	// Flush the message buffer
	if (messageVerbosity >= FlushVerbosity || static_cast<int>(m_FileMessageBuffer.size()) >= BufferMessageCapacity)
	{
		
	}
}

void Logger::AddDestination(const LoggerDestination destination)
{
	Destination |= destination;
}

void Logger::RemoveDestination(const LoggerDestination destination)
{
	Destination &= ~destination;
}

bool Logger::HasDestination(const LoggerDestination destination)
{
	return static_cast<int>(Destination & destination) != 0;
}

std::string Logger::GetFileMessageBufferContents()
{
	std::string result;
	for (auto& message : m_FileMessageBuffer)
	{
		result += message;
	}

	return result;
}

void Logger::FlushFileMessageBuffer()
{
	if (!HasDestination(LoggerDestination::File)) return;

	std::string contents = GetFileMessageBufferContents();
	if (m_FileMessageBuffer.empty() || contents.empty()) return;

	int tries = 0;
	while (true)
	{
		std::string filepath = m_LogFilePath + (tries > 0 ? std::to_string(tries) : "");
		if (!File::Write(filepath, contents, true))
		{
			tries++;
		}
		else
		{
			break;
		}
	}

	m_FileMessageBuffer.clear();
}

std::string Logger::GetMessageHeader(const LoggerVerbosity verbosity, const std::string& category)
{
	std::string verbosityName = s_VerbosityNames.find(verbosity)->second;
	const std::string padding = std::string(GetLongestVerbosityLength() - verbosityName.size(), ' ');
	const std::string paddedVerbosity = padding + verbosityName;

	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	const std::string date = std::ctime(&now);

	return fmt::format(m_MessageHeaderFormat, fmt::arg("date", date), fmt::arg("verbosity", paddedVerbosity), fmt::arg("category", category));
}

std::size_t Logger::GetLongestVerbosityLength()
{
	std::size_t longest = 0;
	for (auto& verbosityName : s_VerbosityNames)
	{
		longest = std::max(verbosityName.second.size(), longest);
	}

	return longest;
}

std::string Logger::GetLogFileName()
{
	std::time_t time = std::time(nullptr);
	std::stringstream stream;
	stream << std::put_time(std::localtime(&time), "%F_%T");

	std::string result = stream.str();
	std::replace(result.begin(), result.end(), ':', '-');
	return result + ".log";
}
