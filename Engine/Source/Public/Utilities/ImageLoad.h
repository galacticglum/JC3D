/**
 * Author: Shon Verch
 * File Name: ImageLoad.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: Utilities for loading images.
 */

#pragma once

#include <string>

struct Image
{
	/**
	 * @brief Loads an image at the specified @p filepath.
	 */
	static unsigned char* Load(const char* filepath, uint32_t* width = nullptr, uint32_t* height = nullptr, uint32_t* components = nullptr, bool flipY = false) const;

	/**
	 * @brief Loads an image at the specified @p filepath.
	 */
	static unsigned char* Load(const std::string& filepath, uint32_t* width = nullptr, uint32_t* height = nullptr, uint32_t* components = nullptr, bool flipY = false) const;
};