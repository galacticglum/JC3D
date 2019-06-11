/**
 * Author: Shon Verch
 * File Name: BufferElement.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: An individual element of a BufferLayout.
 */

#pragma once

#include <utility>

#include <Logger.h>

/**
 * @enum ShaderDataType BufferElement.h
 * @brief The different data types in a shader.
 */
enum class ShaderDataType
{
	None = 0,
	Float,
	Int,
	Bool,
	Vector2f,
	Vector3f,
	Vector4f,
	Vector2i,
	Vector3i,
	Vector4i,
	Matrix3f,
	Matrix4f,
};

/**
 * @brief Gets the size of the specified @p type.
 */
static std::size_t ShaderDataTypeSize(ShaderDataType type)
{
	constexpr int FLOAT_SIZE = 4;
	constexpr int INT_SIZE  = 4;

	switch(type)
	{
		case ShaderDataType::Float: return FLOAT_SIZE;
		case ShaderDataType::Int: return INT_SIZE;
		case ShaderDataType::Bool: return 1;
		case ShaderDataType::Vector2f: return FLOAT_SIZE * 2;
		case ShaderDataType::Vector3f: return FLOAT_SIZE * 3;
		case ShaderDataType::Vector4f: return FLOAT_SIZE * 4;
		case ShaderDataType::Vector2i: return INT_SIZE * 2;
		case ShaderDataType::Vector3i: return INT_SIZE * 3;
		case ShaderDataType::Vector4i: return INT_SIZE * 4;
		case ShaderDataType::Matrix3f: return FLOAT_SIZE * 3 * 3;
		case ShaderDataType::Matrix4f: return FLOAT_SIZE * 4 * 4;
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown ShaderDataType!");
	return 0;
}

 /**
  * @struct BufferElement BufferElement.h
  * @brief An individual element of a BufferLayout.
  */
struct BufferElement
{
	/**
	 * @brief The name of this BufferElement.
	 */
	std::string Name;

	/**
	 * @brief The offset of this BufferElement in the buffer.
	 */
	uint32_t Offset;

	/**
	 * @brief The size of this BufferElement.
	 */
	std::size_t Size;

	/**
	 * @brief The data type of this BufferElement.
	 */
	ShaderDataType Type;

	/**
	 * @brief Indicates whether this BufferElement ia normalized.
	 */
	bool Normalized;
	bool m_Normalized;

	/**
	 * @brief Default constructor for BufferElement.
	 */
	BufferElement() = default;

	/**
	 * @brief Initialize a new BufferElement.
	 */
	BufferElement(const ShaderDataType type, std::string name, const bool normalized = false) :
		Name(std::move(name)), Offset(0), Size(ShaderDataTypeSize(type)), Type(type), Normalized(normalized), m_Normalized(normalized)
	{
	}

	/**
	 * @brief Get the number of components making up this BufferElement.
	 */
	uint32_t GetComponentCount() const
	{
		switch (Type)
		{
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Int: return 1;
			case ShaderDataType::Bool: return 1;
			case ShaderDataType::Vector2f: return 2;
			case ShaderDataType::Vector3f: return 3;
			case ShaderDataType::Vector4f: return 4;
			case ShaderDataType::Vector2i: return 2;
			case ShaderDataType::Vector3i: return 3;
			case ShaderDataType::Vector4i: return 4;
			case ShaderDataType::Matrix3f: return 3 * 3;
			case ShaderDataType::Matrix4f: return 4 * 4;
		}

		LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown ShaderDataType!");
		return 0;
	}
};
