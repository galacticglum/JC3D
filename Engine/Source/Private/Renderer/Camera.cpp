#include <Renderer/Camera.h>
#include <Math/MathFunctions.h>

#include <Input.h>

Camera::Camera(const Matrix4f& projectionMatrix)
	: m_ProjectionMatrix(projectionMatrix), m_Panning(false), m_Rotating(false)
{
	// Sensible defaults
	m_PanSpeed = 0.0015f;
	m_RotationSpeed = 0.002f;
	m_ZoomSpeed = 0.2f;

	m_Position = {-100, 100, 100};
	m_Rotation = Vector3f(90.0f, 0.0f, 0.0f);

	m_FocalPoint = Vector3f();
	m_Distance = Vector3f::Distance(m_Position, m_FocalPoint);

	m_Yaw = 3.0f * MathFunctions::PI_OVER_4;
	m_Pitch = MathFunctions::PI_OVER_4;
}

void Camera::Focus() const
{
}

void Camera::Update()
{
	if (Input::IsKeyPressed(KeyCode::LeftAlt))
	{
		const Vector2f& mouse = Input::GetMousePosition();
		const Vector2f delta = mouse - m_InitialMousePosition;
		m_InitialMousePosition = mouse; 

		if (Input::IsMouseButtonPressed(MouseButton::Middle))
		{
			MousePan(delta);
		}
		else if (Input::IsMouseButtonPressed(MouseButton::Left))
		{
			MouseRotate(delta);
		}
		else if (Input::IsMouseButtonPressed(MouseButton::Right))
		{\
			MouseZoom(delta.Y);
		}
	}

	m_Position = CalculatePosition();

	Quaternion orientation = GetOrientation();
	m_Rotation = orientation.ToEulerAngles() * static_cast<float>(180.0f / MathFunctions::PI);
	m_ViewMatrix = Matrix4f::Translate(Vector3f(0, 0, 1)) * Matrix4f::Rotate(orientation.Conjugate()) * Matrix4f::Translate(-m_Position);
}

void Camera::MousePan(const Vector2f& delta)
{
	m_FocalPoint += -GetRightDirection() * delta.X * m_PanSpeed * m_Distance;
	m_FocalPoint += GetUpDirection() * delta.Y * m_PanSpeed * m_Distance;
}

void Camera::MouseRotate(const Vector2f& delta)
{
	const float yawSign = GetUpDirection().Y < 0 ? -1.0f : 1.0f;
	m_Yaw += yawSign * delta.X * m_RotationSpeed;
	m_Pitch += delta.Y * m_RotationSpeed;
}

void Camera::MouseZoom(const float delta)
{
	m_Distance -= delta * m_ZoomSpeed;
	if (m_Distance < 1.0f)
	{
		m_FocalPoint += GetForwardDirection();
		m_Distance = 1.0f;
	}
}

Vector3f Camera::GetUpDirection() const
{
	return Quaternion::Rotate(GetOrientation(), Vector3f::Up);
}

Vector3f Camera::GetRightDirection() const
{
	return Quaternion::Rotate(GetOrientation(), Vector3f::Right);
}

Vector3f Camera::GetForwardDirection() const
{
	return Quaternion::Rotate(GetOrientation(), Vector3f(0, 0, 1));
}

Vector3f Camera::CalculatePosition() const
{
	return m_FocalPoint - GetForwardDirection() * m_Distance;
}

Quaternion Camera::GetOrientation() const
{
	return Quaternion::FromEulerAngles(Vector3f(-m_Pitch, -m_Yaw, 0.0f));
}
