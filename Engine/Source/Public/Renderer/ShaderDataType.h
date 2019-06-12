/**
 * Author: Shon Verch
 * File Name: ShaderDataType.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/2019
 * Modified Date: 06/12/2019
 * Description: The different data types in a shader.
 */

#pragma once

#include <Logger.h>

/**
 * @enum ShaderDataType BufferElement.h
 * @brief The different data types in a shader.
 */
enum class ShaderDataType
{
	None = 0,
	Float, Float2, Float3, Float4,
	Int, Int2, Int3, Int4, Uint32,
	Bool, Bool2, Bool3, Bool4,
	Matrix3x3, Matrix4x4,
};

/**
 * @brief Get the number of components making up this BufferElement.
 */
static uint32_t GetShaderDataTypeComponentCount(const ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Int: return 1;
		case ShaderDataType::Int2: return 2;
		case ShaderDataType::Int3: return 3;
		case ShaderDataType::Int4: return 4;
		case ShaderDataType::Uint32: return 1;
		case ShaderDataType::Bool: return 1;
		case ShaderDataType::Bool2: return 2;
		case ShaderDataType::Bool3: return 3;
		case ShaderDataType::Bool4: return 4;
		case ShaderDataType::Matrix3x3: return 3 * 3;
		case ShaderDataType::Matrix4x4: return 4 * 4;
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown ShaderDataType!");
	return 0;
}

/**
 * @brief Gets the size of the specified @p type.
 */
static std::size_t ShaderDataTypeSize(const ShaderDataType type)
{
	constexpr int FLOAT_SIZE = 4;
	constexpr int INT_SIZE = 4;
	constexpr int BOOL_SIZE = 1;

	switch (type)
	{
		case ShaderDataType::Float: return FLOAT_SIZE;
		case ShaderDataType::Float2: return FLOAT_SIZE * 2;
		case ShaderDataType::Float3: return FLOAT_SIZE * 3;
		case ShaderDataType::Float4: return FLOAT_SIZE * 4;
		case ShaderDataType::Int: return INT_SIZE;
		case ShaderDataType::Int2: return INT_SIZE * 2;
		case ShaderDataType::Int3: return INT_SIZE * 3;
		case ShaderDataType::Int4: return INT_SIZE * 4;
		case ShaderDataType::Uint32: return INT_SIZE;
		case ShaderDataType::Bool: return BOOL_SIZE;
		case ShaderDataType::Bool2: return BOOL_SIZE * 2;
		case ShaderDataType::Bool3: return BOOL_SIZE * 3;
		case ShaderDataType::Bool4: return BOOL_SIZE * 4;
		case ShaderDataType::Matrix3x3: return FLOAT_SIZE * 3 * 3;
		case ShaderDataType::Matrix4x4: return FLOAT_SIZE * 4 * 4;
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown ShaderDataType!");
	return 0;
}