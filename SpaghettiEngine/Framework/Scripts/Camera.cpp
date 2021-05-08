#include "Camera.h"
#include "Setting.h"
#include "Graphics.h"
#include "GraphicsMath.h"
#include "LoadingJson.h"

REGISTER_FINISH(Camera);

Camera::Camera(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(Camera);
	GraphicsMath::ZeroMatrix(&viewMatrix);
	viewMatrix._11 = 1;
	viewMatrix._22 = -1;
	viewMatrix._33 = 1;
	viewMatrix._44 = 1;

	GraphicsMath::ZeroMatrix(&flipYMatrix);
	flipYMatrix._11 = 1;
	flipYMatrix._22 = -1;
	flipYMatrix._33 = 1;
	flipYMatrix._44 = 1;
}

Matrix Camera::GetMatrix(const Matrix& originalMatrix)
{
	if (needRecalculateMatrix)
	{
		needRecalculateMatrix = false;
		GraphicsMath::Inverse(GetWorldMatrix(), cameraMatrix);
		cameraMatrix *= viewMatrix;
	}
	return flipYMatrix * originalMatrix * cameraMatrix;
}

void Camera::OnUpdate()
{
	if (_followingPtr.use_count() > 0)
	{
		const Vector3 delta = _followingObj->GetWorldTransform() - _ownerObj->GetWorldTransform();
		_ownerObj->Translate(delta * _dragFactor);
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
		_followingPtr = _owner->GetComponent(input[LoadingJson::Field::gameObjectsField][0][Field::idField]);
		_followingObj = (dynamic_cast<PGameObj>(_followingPtr.lock().get()));
	}
}

void Camera::SetFollow(PGameObj followObj)
{
	_followingPtr = followObj->GetSharedPtr();
	_followingObj = (dynamic_cast<PGameObj>(_followingPtr.lock().get()));
}

PGameObj Camera::GetFollow()
{
	return _followingObj;
}

void Camera::RemoveFollow()
{
	_followingObj = nullptr;
	_followingPtr.reset();
}
