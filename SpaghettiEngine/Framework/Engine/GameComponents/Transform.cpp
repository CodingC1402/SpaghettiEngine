#include "Transform.h"
#include "SMath.h"
#include "ContainerUtil.h"
#include "GameObj.h"

#pragma region Transform Getters
PTransform Transform::GetParent() const
{
	return _parentTransform;
}
Matrix4 Transform::GetWorldMatrix()
{
	CalculateWorldMatrix();
	return _worldMatrix;
}
Vector3 Transform::GetWorldTransform() const
{
	if (_parentTransform)
		return _transform + _parentTransform->GetWorldTransform();
	else
		return _transform;
}
Vector3 Transform::GetWorldRotation() const
{
	if (_parentTransform)
		return _rotation + _parentTransform->GetWorldRotation();
	else
		return _rotation;
}
Vector3 Transform::GetWorldScale() const
{
	Vector3 rValue = _scale;
	if (_parentTransform)
	{
		const Vector3 parentValue = _parentTransform->GetWorldScale();
		rValue = Vector3(rValue.x * parentValue.x, rValue.y * parentValue.y, rValue.y * parentValue.y);
	}
	return rValue;
}
Vector3 Transform::GetTransform() const
{
	return _transform;
}
Vector3 Transform::GetRotation() const
{
	return _rotation;
}
Vector3 Transform::GetScale() const
{
	return _scale;
}
#pragma endregion

//=======================================================================================================================//

#pragma region Transform Setters
void Transform::SetParent(PTransform parent)
{
	if (!parent)
		if (_parentTransform)
		{
			_parentTransform->RemoveChild(this);
			_parentTransform = nullptr;
		}
	else
		parent->AddChild(this);
}
void Transform::AddChild(PTransform child)
{
	if (child->_parentTransform == this)
		return;

	if (child->_parentTransform)
		_parentTransform->RemoveChild(child);

	if (this->_parentTransform == child)
		throw CORN_EXCEPTION_TYPE(TransformException, L"You can't set a parent of a transform to child of that transform");

	_childTransform.push_back(child);
	child->_parentTransform = this;

	child->_transform -= GetWorldTransform();
	child->_rotation -= GetWorldRotation();

	const Vector3 parentScale = GetWorldScale();
	child->_scale.x /= parentScale.x;
	child->_scale.y /= parentScale.y;
	child->_scale.z /= parentScale.z;

	child->ForceRecalculateMatrix();
}
void Transform::RemoveChild(PTransform child)
{
	if (!ContainerUtil::Erase(_childTransform, child))
		return;

	child->_parentTransform = nullptr;
	child->_transform += GetWorldTransform();
	child->_rotation += GetWorldRotation();

	const Vector3 parentScale = GetWorldScale();
	child->_scale.x *= parentScale.x;
	child->_scale.y *= parentScale.y;
	child->_scale.z *= parentScale.z;

	child->ForceRecalculateMatrix();
}
void Transform::SetRotation(const float& x, const float& y, const float& z)
{
	SetRotation(Vector3(x, y, z));
}
void Transform::SetRotation(const Vector3& vec3)
{
	if (_rotation == vec3)
	{
		_rotation = vec3;
		SMath::Modulo(_rotation, 360.0f);
		_isRotationChanged = true;
		ForceRecalculateMatrix();
	}
}
void Transform::SetScale(const float& x, const float& y, const float& z)
{
	SetScale(Vector3(x, y, z));
}
void Transform::SetScale(const Vector3& vec3)
{
	if (_scale != vec3)
	{
		_scale.x = vec3.x;
		_scale.y = vec3.y;
		_scale.z = vec3.z;
		_isScaleChanged = true;
		ForceRecalculateMatrix();
	}
}
void Transform::SetTransform(const float& x, const float& y, const float& z)
{
	SetTransform(Vector3(x, y, z));
}
void Transform::SetTransform(const Vector3& vec3)
{
	if (_transform != vec3)
	{
		_transform = vec3;
		_isTransformChanged = true;
		ForceRecalculateMatrix();
	}
}
void Transform::Rotate(const float& x, const float& y, const float& z)
{
	Rotate(Vector3(x, y, z));
}
void Transform::Rotate(const Vector3& rotation)
{
	if (rotation == Vector3(0, 0, 0))
		return;

	_rotation += rotation;
	SMath::Modulo(_rotation, 360.0f);
	_isRotationChanged = true;
	ForceRecalculateMatrix();
}

void Transform::Move(const Vector3& vector)
{
	if (vector == Vector3(0, 0, 0))
		return;

	_transform = vector;
	_isTransformChanged = true;
	ForceRecalculateMatrix();
}

void Transform::Move(const float& x, const float& y, const float& z)
{
	Move(Vector3(x, y, z));
}

void Transform::Translate(const float& x, const float& y, const float& z)
{
	Translate(Vector3(x, y, z));
}
Transform::Transform(PGameObj owner)
{
	_owner = owner;

	_transformMatrix = Matrix4::GetDiagonalMatrix();
	_rotationMatrix = Matrix4::GetDiagonalMatrix();
	_scaleMatrix = Matrix4::GetDiagonalMatrix();
}
void Transform::Translate(const Vector3& vector)
{
	if (vector == Vector3(0, 0, 0))
		return;

	_transform += vector;
	_isTransformChanged = true;
	ForceRecalculateMatrix();
}
void Transform::ForceRecalculateMatrix()
{
	this->_isChanged = true;
	if (_owner->GetTag() == "Player")
	{
		int oneH = 100;
	}
	for (auto& child : _childTransform)
		child->ForceRecalculateMatrix();
}
#pragma endregion

//======================================================================================================================//

#pragma region Matrixes
void Transform::CalculateRotationMatrix()
{
	if (!_isRotationChanged)
		return;

	const Matrix4 XAxis = SMath::GetXAxisRotateMatrix(_rotation.x);
	const Matrix4 YAxis = SMath::GetYAxisRotateMatrix(_rotation.y);
	const Matrix4 ZAxis = SMath::GetZAxisRotateMatrix(_rotation.z);

	_rotationMatrix = XAxis * ZAxis * YAxis;
	_isRotationChanged = false;
}
void Transform::CalculateTransformMatrix()
{
	if (!_isTransformChanged)
		return;

	_transformMatrix._41 = _transform.x;
	_transformMatrix._42 = _transform.y;
	_transformMatrix._43 = _transform.z;
	_transformMatrix._11 = 1;
	_transformMatrix._22 = 1;
	_transformMatrix._33 = 1;
	_transformMatrix._44 = 1;
	_isTransformChanged = false;
}
void Transform::CalculateScaleMatrix()
{
	if (!_isScaleChanged)
		return;

	_scaleMatrix._11 = _scale.x;
	_scaleMatrix._22 = _scale.y;
	_scaleMatrix._33 = _scale.z;
	_scaleMatrix._44 = 1;
	_isScaleChanged = false;
}
void Transform::CalculateWorldMatrix()
{
	if (!_isChanged)
		return;

	CalculateRotationMatrix();
	CalculateTransformMatrix();
	CalculateScaleMatrix();

	if (_parentTransform)
		_worldMatrix = _scaleMatrix * _rotationMatrix * _transformMatrix * _parentTransform->GetWorldMatrix();
	else
		_worldMatrix = _scaleMatrix * _rotationMatrix * _transformMatrix;

	_isChanged = false;
}
#pragma endregion

Transform::TransformException::TransformException(int line, const char* file, const std::wstring& des) noexcept : CornException(line, file)
{
	_description = des;
};

const wchar_t* Transform::TransformException::What() const noexcept
{
	std::wostringstream oss;
	oss << CornException::What();
	oss << "[Description] " << _description.c_str() << std::endl;
	return whatBuffer.c_str();
}

const wchar_t* Transform::TransformException::GetType() const noexcept
{
	return L"Transform exception";
}
