/**
 * Author: Shon Verch
 * File Name: Mesh.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/13/2019
 * Modified Date: 06/13/2019
 * Description: A data-structure for representing a three-dimensional model.
 */

#pragma once

#include <memory>
#include <vector>

#include <Math/Vector.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/IndexBuffer.h>

/**
 * @class Mesh Mesh.h
 * @brief A data-structure for representing a three-dimensional model.
 */
class Mesh
{
public:
	/**
	 * @brief A vertex of the model.
	 */
	struct Vertex
	{
		Vector3f Position;
		Vector3f Normal;
		Vector3f Tangent;
		Vector3f Binormal;
		Vector2f TextureCoordinate;
	};

	/**
	 * @brief The number of attributes in this Mesh.
	 */
	static const int AttributeCount = 5;

	struct Index
	{
		uint32_t V1, V2, V3;
	};

	/**
	 * @brief Initialize a new Mesh.
	 */
	explicit Mesh(const std::string& filepath);
	~Mesh();

	void Render();

	const std::string& GetFilepath() const
	{
		return m_Filepath;
	}
private:
	std::vector<Vertex> m_Vertices;
	std::vector<Index> m_Indices;

	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;

	std::string m_Filepath;
};