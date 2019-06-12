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

#include <Renderer/ShaderDataType.h>

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
		Name(std::move(name)), Offset(0), Size(ShaderDataTypeHelper::GetSize(type)), Type(type), Normalized(normalized), m_Normalized(normalized)
	{
	}

	/**
	 * @brief Get the number of components making up this BufferElement.
	 */
	uint32_t GetComponentCount() const
	{
		return ShaderDataTypeHelper::GetComponentCount(Type);
	}
};
