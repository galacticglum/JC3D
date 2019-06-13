/**
 * Author: Shon Verch
 * File Name: Shader.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/12/2019
 * Description: Shader scripts used for rendering.
 */

#pragma once

#include <cstddef>
#include <string>
#include <type_traits>

#include <Renderer/Renderer.h>
#include <Renderer/ShaderDataType.h>

#include <Math/Vector.h>
#include <Math/Matrix.h>

/**
 * @struct ShaderUniformDeclaration Shader.h
 * @brief A shader uniform declaration.
 * @note Used for binding the uniform to the shader.
 */
struct ShaderUniformDeclaration
{
	/**
	 * @brief The type of the uniform variable.s
	 */
	ShaderDataType Type;

	/**
	 * @brief The uniform variable offset.
	 */
	std::ptrdiff_t Offset;

	/**
	 * @brief The name of the uniform variable.
	 */
	std::string Name;
};

/**
 * @struct ShaderUniformBufferBase Shader.h
 * @brief A byte buffer packed with uniforms.
 * @note This is simply a CPU-side buffer abstraction.
 */
struct ShaderUniformBufferBase
{
	/**
	 * @brief Destroy this ShaderUniformBufferBase.
	 */
	virtual ~ShaderUniformBufferBase() = default;

	/**
	 * @brief Gets the uniform buffer.
	 */
	virtual const unsigned char* GetBuffer() const = 0;

	/**
	 * @brief Gets the uniform declarations.
	 */
	virtual const ShaderUniformDeclaration* GetUniforms() const = 0;

	/**
	 * @brief Gets the number of uniforms in this ShaderUniformBufferBase.
	 */
	virtual std::size_t GetUniformCount() const = 0;
};

/**
 * @struct ShaderUniformBufferDeclaration Shader.h
 * @brief Implementation of the ShaderUniformBufferBase.
 * @tparam N The size of the buffer.
 * @tparam U The number of uniforms in this buffer.
 */
template<uint32_t N, uint32_t U>
struct ShaderUniformBufferDeclaration : public ShaderUniformBufferBase
{	
	/**
	 * @brief The uniform buffer.
	 */
	unsigned char Buffer[N]{};

	/**
	 * @brief The uniforms of this ShaderUniformBufferDeclaration.
	 */
	ShaderUniformDeclaration Uniforms[U];

	/**
	 * @brief The access cursor.
	 * @note Used for seeking various parts of the buffer.
	 */
	std::ptrdiff_t Cursor = 0;

	/**
	 * @brief The insertion index for the ShaderUniformBufferDeclaration::Uniforms array.
	 */
	int Index = 0;

	/**
	 * @brief Gets the byte buffer for this ShaderUniformBufferDeclaration.
	 */
	const unsigned char* GetBuffer() const override
	{
		return Buffer;
	}

	/**
	 * @brief Gets the uniforms for this ShaderUniformBufferDeclaration.
	 */
	const ShaderUniformDeclaration* GetUniforms() const override
	{
		return Uniforms;
	}

	/**
	 * @brief Gets the number of uniforms in this ShaderUniformBufferDeclaration.
	 */
	std::size_t GetUniformCount() const override
	{
		return U;
	}

	/**
	 * @brief Push a uniform of type @p T with the specified @p name onto this ShaderUniformBufferDeclaration.
	 */
	template <typename T>
	void Push(const std::string& name, const T& data)
	{
		// Check whether the templated type is supported and which
		// ShaderDataType it aligns with. Since this is doing a direct
		// type check, implicit type-casting is not supported.

		// This is a little hacky but its the only way to do it...
		// Fortunately, the ugly code is mostly abstracted into
		// the ShaderDataType implementation.

		ShaderDataType type = ShaderDataTypeHelper::FromType<T>();

		Uniforms[Index++] = { type, Cursor, name };

		void const* ptr;
		std::size_t size;

		// Since we got the shader type from a runtime type (which is a function that is defined by the
		// engine), we can be sure that if type is a vector or matrix type, that it INHERITS from the
		// Vector or Matrix base types defined in the engine maths library.
		if (ShaderDataTypeHelper::IsVectorType(type) || ShaderDataTypeHelper::IsMatrixType(type))
		{
			// If the data is a vector or matrix, our ptr needs to point to the vector/matrix elements.
			// data should have a Data member since it is a vector or matrix.
			ptr = &data.Data;

			// Use the Data member for the size to ensure that no memory padding is included.
			size = sizeof(data.Data);
		}
		else
		{
			ptr = &data;
			size = sizeof(data);
		}

		memcpy(Buffer + Cursor, ptr, size);
		Cursor += size;
	}
};

/**
 * @class Shader Shader.h
 * @brief Shader scripts used for rendering.
 */
class Shader
{
public:
	/**
	 * @brief Destroy this Shader.
	 */
	virtual ~Shader() = default;

	/**
	 * @brief Reload this shader.
	 */
	virtual void Reload() = 0;

	/**
	 * @brief Bind this Shader.
	 */
	virtual void Bind() const = 0;

	/**
	 * @brief Unbind this Shader.
	 */
	virtual void Unbind() const = 0;

	/**
	 * @brief Get the name of this shader.
	 */
	virtual const std::string& GetName() const = 0;

	/**
	 * @brief Get the filepath of this shader.
	 */
	virtual const std::string& GetFilepath() const = 0;

	/**
	 * @brief Upload a uniform buffer onto this Shader.
	 */
	virtual void UploadUniformBuffer(const ShaderUniformBufferBase& uniformBuffer) const = 0;

	/**
	 * @brief Set the value of the integer uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, int value) const = 0;

	/**
	 * @brief Set the value of the float uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, float value) const = 0;

	/**
	 * @brief Set the value of the boolean uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, bool value) const = 0;

	/**
	 * @brief Set the value of the Vector2f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Vector2f& value) const = 0;

	/**
	 * @brief Set the value of the Vector3f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Vector3f& value) const = 0;

	/**
	 * @brief Set the value of the Vector4f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Vector4f& value) const = 0;

	/**
	 * @brief Set the value of the Matrix4f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Matrix4f& value) const = 0;

	/**
	 * @brief Create a new Shader.
	 * @param filepath The path to the shader.
	 */
	static Shader* Create(const std::string& filepath);

	/**
	 * @brief All the shaders.
	 */
	static std::vector<Shader*> s_AllShaders;
};