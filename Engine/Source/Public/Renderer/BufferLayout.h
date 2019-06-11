/**
 * Author: Shon Verch
 * File Name: BufferLayout.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: Buffer layout descriptor.
 */

#pragma once

#include <initializer_list>
#include <vector>

#include <Renderer/BufferElement.h>

/**
 * @class BufferLayout BufferLayout.h
 * @brief Describes the layout of a buffer.
 */
class BufferLayout
{
public:
	/**
	 * @brief Initialize a new BufferLayout from a std::initializer_list<BufferElement>
	 */
	BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
	{
		CalculateLayout();
	}

	/**
	 * @brief Gets the elements of this BufferLayout.
	 */
	const std::vector<BufferElement>& GetElements() const
	{
		return m_Elements;
	}
private:
	/**
	 * @brief Calculates the layout information for this BufferLayout.
	 */
	void CalculateLayout()
	{
		uint32_t offset = 0;
		m_Stride = 0;

		for (BufferElement& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride;
};

