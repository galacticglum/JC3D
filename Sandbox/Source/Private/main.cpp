/**
 * Author: Shon Verch
 * File Name: main.cpp
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/12/2019
 * Description: Entry point for the sandbox application.
 */

#include <Application.h>
#include <PBRLayer.h>

#include <Math/Matrix.h>
#include <Math/Vector.h>

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new PBRLayer());
	}

	~Sandbox() = default;
};

/**
 * Entry point for the sandbox application.
 */
int main()
{
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;

	/**
	 * 7 3 3 1
	 * 0 1 2 8
	 * 1 2 3 1
	 * 1 1 1 1
	 * det(M) = -32
	 */
	const Matrix4f testMatrix = Matrix4f({
		7, 3, 3, 1,
		0, 1, 2, 8,
		1, 2, 3, 1,
		1, 1, 1, 1
	});

	const Matrix4f testMatrixB = Matrix4f({
		1, 1, 1, 1,
		0, 1, 0, 0,
		0, 1, 0, 0,
		0, 1, 0, 0
	});

	auto v = Vector3f(27, 56, 34);
	Quaternion q1 = Quaternion(Vector3f(5, 1, 3), 2);
	Quaternion q2 = Quaternion(Vector3f(8, 2, 3), 7);

	Logger::Log(LoggerVerbosity::Info, "Quat mul {0}", q1 * q2);
	Logger::Log(LoggerVerbosity::Info, "Quat {0}", Quaternion(v));
	Logger::Log(LoggerVerbosity::Info, "euler {0}", Quaternion(v).ToEulerAngles());
	Logger::Log(LoggerVerbosity::Info, "Rotate1 {0}", 
		Matrix4f::Rotate(Quaternion(Vector3f(27, 56, 34))));
}