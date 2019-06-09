/**
 * Author: Shon Verch
 * File Name: File.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/08/2019
 * Modified Date: 06/08/2019
 * Description: File system utilities.
 */

#pragma once

#include <string>

/**
 * @class File File.h
 * @brief File system utilities.
 */
class File
{
public:
	/**
	 * @brief Read the contents of the file at the specified @p filepath.
	 * @param filepath The path to the file to read.
	 */
	static std::string Read(const std::string& filepath);

	/**
	 * @brief Write the specified @p contents to the file at the specified @p filepath.
	 * @param filepath The path to the file to write in.
	 * @param content The content to write in the file.
	 * @param overwrite A boolean value indicating whether the file should be overwritten.
	 */
	static bool Write(const std::string& filepath, const std::string& content, bool overwrite = false);

	/**
	 * @brief Determines whether or not the file at the specified @P filepath exists.
	 * @param filepath The path to the file.
	 */
	static bool Exists(const std::string& filepath);

	/**
	 * @brief Deletes the file at the specified @p filepath if it exists.
	 * @param filepath The path to the file.
	 */
	static bool Delete(const std::string& filepath);

	/**
	 * @brief Gets the extension of the file at the specified @p filepath.
	 * @param filepath The path to the file.
	 */
	static std::string GetExtension(const std::string& filepath);
};
