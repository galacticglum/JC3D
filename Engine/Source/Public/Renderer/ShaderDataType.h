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
#include <Math/Vector.h>
#include <Math/Matrix.h>

/**
 * @enum ShaderDataType ShaderDataType.h
 * @brief The different data types in a shader.
 */
enum class ShaderDataType
{
	None = 0, Unknown = 1,
	Float, Float2, Float3, Float4,
	Int, Int2, Int3, Int4, Uint32,
	Bool, Bool2, Bool3, Bool4,
	Matrix3x3, Matrix4x4,
};

/**
 * @brief A mapping of ShaderDataTypes to their respective C++ types.
 */
template <typename T>
struct ShaderDataTypeMapping
{
	static constexpr ShaderDataType Type = ShaderDataType::Unknown;
};

/**
 * @brief A macro for mapping a C++ type to a ShaderDataType.
 */
#define MAP_SHADER_DATA_TYPE(cppType, shaderDataType)			            \
template<>														            \
struct ShaderDataTypeMapping<cppType>							            \
{																            \
	static constexpr ShaderDataType Type = ShaderDataType::shaderDataType;  \
};																			\

MAP_SHADER_DATA_TYPE(std::nullptr_t, None)

MAP_SHADER_DATA_TYPE(float, Float)
MAP_SHADER_DATA_TYPE(Vector2f, Float2)
MAP_SHADER_DATA_TYPE(Vector3f, Float3)

MAP_SHADER_DATA_TYPE(int, Int)
MAP_SHADER_DATA_TYPE(Vector2i, Int2)
MAP_SHADER_DATA_TYPE(Vector3i, Int3)
MAP_SHADER_DATA_TYPE(Vector4i, Int4)
MAP_SHADER_DATA_TYPE(uint32_t, Uint32)

MAP_SHADER_DATA_TYPE(bool, Bool)
MAP_SHADER_DATA_TYPE(Vector2<bool>, Bool2)
MAP_SHADER_DATA_TYPE(Vector3<bool>, Bool3)
MAP_SHADER_DATA_TYPE(Vector4<bool>, Bool4)

MAP_SHADER_DATA_TYPE(Matrix3f, Matrix3x3)
MAP_SHADER_DATA_TYPE(Matrix4f, Matrix4x4)

/**
 * @struct ShaderDataTypeHelper ShaderDataType.h
 * @brief A collection of helper functionality related to the ShaderDataType enum.
 */
struct ShaderDataTypeHelper
{
	/**
	 * @brief Get the number of components making up this BufferElement.
	 */
	static uint32_t GetComponentCount(const ShaderDataType type)
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
	static std::size_t GetSize(const ShaderDataType type)
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

	/**
	 * @brief Gets a ShaderDataType from a std::type_index.
	 */
	template <typename T>
	static ShaderDataType FromType()
	{
		ShaderDataType type = ShaderDataTypeMapping<T>::Type;
		LOG_CATEGORY_ASSERT(type != ShaderDataType::Unknown && type != ShaderDataType::None, 
			"Renderer", "Invalid type provided for ShaderDataType conversion!");

		return type;
	}

	/**
	 * @brief Gets whether the specified @p type is a vector type.
	 */
	static bool IsVectorType(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float2: return true;
			case ShaderDataType::Float3: return true;
			case ShaderDataType::Float4: return true;
			case ShaderDataType::Int2: return true;
			case ShaderDataType::Int3: return true;
			case ShaderDataType::Int4: return true;
			case ShaderDataType::Bool2: return true;
			case ShaderDataType::Bool3: return true;
			case ShaderDataType::Bool4: return true;
		}

		return false;
	}

	/**
	 * @brief Gets whether the specified @p type is a vector type.
	 */
	static bool IsMatrixType(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Matrix3x3: return true;
			case ShaderDataType::Matrix4x4: return true;
		}

		return false;
	}
};