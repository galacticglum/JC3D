/**
 * Author: Shon Verch
 * File Name: Camera.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: Camera implementation.
 */

#pragma once

#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Math/Quaternion.h>

/**
 * @class Camera Camera.h
 * @brief Camera implementation.
 */
class Camera
{
public:
	explicit Camera(Matrix4f projectionMatrix);

	void Focus() const;
	void Update();

	float GetDistance() const
	{
		return m_Distance;
	}

	void SetDistance(const float distance)
	{
		m_Distance = distance;
	}

	void SetProjectionMatrix(const Matrix4f& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
	}

	const Matrix4f& GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	Matrix4f GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	Vector3f GetUpDirection() const;
	Vector3f GetRightDirection() const;
	Vector3f GetForwardDirection() const;
	const Vector3f& GetPosition() const
	{
		return m_Position;
	}
private:
	Matrix4f m_ProjectionMatrix;
	Matrix4f m_ViewMatrix;
	Vector3f m_Position;
	Vector3f m_Rotation;
	Vector3f m_FocalPoint;

	bool m_Panning;
	bool m_Rotating;
	Vector2f m_InitialMousePosition;
	Vector3f m_InitialFocalPoint;
	Vector3f m_InitialRotation;

	float m_Distance;
	float m_PanSpeed;
	float m_RotationSpeed;
	float m_ZoomSpeed;

	float m_Pitch;
	float m_Yaw;

	void MousePan(const Vector2f& delta);
	void MouseRotate(const Vector2f& delta);
	void MouseZoom(float delta);

	Vector3f CalculatePosition() const;
	Quaternion GetOrientation() const;
};