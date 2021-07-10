#include "Camera.h"
#include "Setting.h"
#include "Graphics.h"
#include "LoadingJson.h"
#include "GameTimer.h"
#include "SMath.h"

REGISTER_FINISH(Camera, ScriptBase) 
{
	viewMatrix._11 = 1;
	viewMatrix._22 = -1;
	viewMatrix._33 = 1;
	viewMatrix._44 = 1;

	flipYMatrix._11 = 1;
	flipYMatrix._22 = -1;
	flipYMatrix._33 = 1;
	flipYMatrix._44 = 1;
}

Matrix4 Camera::GetMatrix(const Matrix4& originalMatrix)
{
	return flipYMatrix * GetMatrixWithoutScaleY(originalMatrix);
}

Matrix4 Camera::GetMatrixWithoutScaleY(const Matrix4& originalMatrix)
{
	if (needRecalculateMatrix)
	{
		needRecalculateMatrix = false;
		cameraMatrix = GetWorldMatrix().Inverse();
		cameraMatrix *= viewMatrix;
	}
	return originalMatrix * cameraMatrix;
}

void Camera::OnFixedUpdate()
{
	if (_followingPtr.use_count() > 0)
	{
		const Vector3 delta = _followingPtr.lock()->GetTransform().GetWorldTransform() - GetGameObject()->GetTransform().GetWorldTransform();
		GetGameObject()->GetTransform().Translate(delta);
	}
	viewMatrix._41 = static_cast<float>(Setting::GetResolution().width) / 2.0f;
	viewMatrix._42 = static_cast<float>(Setting::GetResolution().height) / 2.0f;
	needRecalculateMatrix = true;
}

void Camera::OnDisabled()
{
	Graphics::RemoveCamera(this);
}

void Camera::OnEnabled()
{
	Graphics::AddCamera(this);
}

void Camera::Load(json& input)
{
	using LoadingJson::Field;
	if (!input[LoadingJson::Field::gameObjectsField].empty())
	{
		auto gameObj = dynamic_cast<PGameObj>(GetOwner()->GetComponent(input[LoadingJson::Field::gameObjectsField][0][Field::idField]));
		_followingPtr = std::dynamic_pointer_cast<GameObj>(gameObj->GetSharedPtr());
	}
}

void Camera::SetFollow(PGameObj followObj)
{
	_followingPtr = std::dynamic_pointer_cast<GameObj>(followObj->GetSharedPtr());
}

PGameObj Camera::GetFollow()
{
	if (_followingPtr.expired())
		return nullptr;
	return _followingPtr.lock().get();
}

void Camera::RemoveFollow()
{
	_followingPtr.reset();
}

PScriptBase Camera::Clone() const
{
	auto clone = dynamic_cast<Camera*>(ScriptBase::Clone());

	clone->_followingPtr = _followingPtr;
	clone->oldH = oldH;
	clone->oldW = oldW;

	return clone;
}
