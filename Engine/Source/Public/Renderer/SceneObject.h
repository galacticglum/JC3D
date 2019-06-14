/**
 * Author: Shon Verch
 * File Name: SceneObject.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/13/19
 * Modified Date: 06/13/19
 * Description: An object in the scene.
 */

#pragma once

#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Math/Matrix.h>

#include <Renderer/Mesh.h>
#include <Renderer/Material.h>

/**
 * @struct SceneObject SceneObject.h
 * @brief An object in the scene.
 */
struct SceneObject
{
	/**
	 * @brief Default initialize this SceneObject.
	 * @note This will set the mesh to null.
	 */
	explicit SceneObject(Material* material = new Material());

	/**
	 * @brief The position of this SceneObject.
	 */
	Vector3f Position;

	/**
	 * @brief The scale of this SceneObject.
	 */
	Vector3f Scale;

	/**
	 * @brief The rotation of this SceneObject.
	 */
	Quaternion Rotation;

	/**
	 * @brief Calculates the transformation matrix for this SceneObject.
	 */
	Matrix4f GetTransformationMatrix() const;

	/**
	 * @brief Get this SceneObject's mesh.
	 */
	std::shared_ptr<Mesh> GetMesh() const
	{
		return m_Mesh;
	}

	/**
	 * @brief Set this SceneObject's mesh.
	 */
	void SetMesh(Mesh* mesh)
	{
		m_Mesh.reset(mesh);
	}

	/**
	 * @brief Get this SceneObject's material.
	 */
	std::shared_ptr<Material> GetMaterial() const
	{
		return std::shared_ptr<Material>(m_Material);
	}

	/**
	 * @brief Set this SceneObject's material.
	 */
	void SetMaterial(Material* material)
	{
		m_Material.reset(material);
	}
private:
	/**
	 * @brief The mesh of this SceneObject.
	 */
	std::shared_ptr<Mesh> m_Mesh;

	/**
	 * @brief The material of this SceneObject.
	 */
	std::shared_ptr<Material> m_Material;
};