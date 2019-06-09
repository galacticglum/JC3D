#include <System/File.h>
#include <Utilities/Logger.h>

#include <fstream>
#include <cstdio>

std::string File::Read(const std::string& filepath)
{
	std::ifstream file;
	file.open(filepath);

	std::string contents;
	std::string lineBuffer;
	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, lineBuffer);
			contents.append(lineBuffer + "\n");
		}

		return contents;
	}
	
	// TODO: Log error; "Could not open file: " + filepath + "!\n"
	return contents;
}

bool File::Write(const std::string& filepath, const std::string& content, const bool overwrite)
{
	std::ofstream file;
	file.open(filepath);

	if (file.is_open())
	{
		if (overwrite)
		{
			file.clear();
		}

		file << content;
		return true;
	}
	
	// TODO: Log error; "Could not open file: " + filepath+ "!\n"
	return false;
}

bool File::Exists(const std::string& filepath)
{
	std::ofstream file;
	file.open(filepath);

	return file.is_open();
}

bool File::Delete(const std::basic_string<char>& filepath)
{
	if (!Exists(filepath)) return false;
	std::remove(filepath.c_str());
}


std::string File::GetExtension(const std::string& filepath)
{
	const auto dotIndex = filepath.find_last_of('.');
	if (dotIndex != std::string::npos)
	{
		return filepath.substr(dotIndex + 1);
	}

	return std::string();
}