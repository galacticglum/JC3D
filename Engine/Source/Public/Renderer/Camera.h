/**
 * Author: Shon Verch
 * File Name: Camera.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: Base camera.
 */

#pragma once

#include <cmath>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Math/MathFunctions.h>

enum class CameraMovement
{
	Forward,
	Backward,
	Left,
	Right
};

#define DEFAULT_YAW -90.0f
#define DEFAULT_PITCH 0.0f
#define DEFAULT_SPEED 2.5f
#define DEFAULT_SENSITIVITY 0.1f
#define DEFAULT_ZOOM 45.0f

class Camera
{
public:
	Vector3f Position;
	Vector3f Front;
	Vector3f Up;
	Vector3f Right;
	Vector3f WorldUp;

	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	explicit Camera(Vector3f position = Vector3f(0, 0, 0), Vector3f up = Vector3f(0, 1, 0), float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH)
		: Front(Vector3f(0, 0, -1)), MovementSpeed(DEFAULT_SPEED), MouseSensitivity(DEFAULT_SENSITIVITY), Zoom(DEFAULT_ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
		: Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(DEFAULT_SPEED), MouseSensitivity(DEFAULT_SENSITIVITY), Zoom(DEFAULT_ZOOM)
	{
		Position = Vector3f(posX, posY, posZ);
		WorldUp = Vector3f(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	Matrix4f GetViewMatrix() const
	{
		return Matrix4f::LookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, float deltaTime)
	{
		const float velocity = MovementSpeed * deltaTime;
		if (direction == CameraMovement::Forward)
			Position += Front * velocity;
		if (direction == CameraMovement::Backward)
			Position -= Front * velocity;
		if (direction == CameraMovement::Left)
			Position -= Right * velocity;
		if (direction == CameraMovement::Right)
			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		UpdateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}
private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors()
	{
		// Calculate the new Front vector
		Vector3f front;
		front.X = std::cos(MathFunctions::DegreesToRadians(Yaw)) * std::cos(MathFunctions::DegreesToRadians(Pitch));
		front.Y = std::sin(MathFunctions::DegreesToRadians(Pitch));
		front.Z = std::sin(MathFunctions::DegreesToRadians(Yaw)) * std::cos(MathFunctions::DegreesToRadians(Pitch));
		Front = Vector3f::Normalize(front);
		// Also re-calculate the Right and Up vector
		Right = Vector3f::Cross(Front, WorldUp).Normalized();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = Vector3f::Cross(Right, Front).Normalized();
	}
};