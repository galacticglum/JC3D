#include <Renderer/SceneObject.h>

SceneObject::SceneObject(Material* material)
{
	m_Mesh = nullptr;
	m_Material = nullptr;

	SetMaterial(material);
}

Matrix4f SceneObject::GetTransformationMatrix() const
{
	const Matrix4f scale = Matrix4f::Scale(Scale);
	const Matrix4f rotation = Matrix4f::Rotate(Rotation);
	const Matrix4f translation = Matrix4f::Translate(Position);

	return scale * rotation * translation;
}
