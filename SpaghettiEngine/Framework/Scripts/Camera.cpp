#include "Camera.h"
#include "Setting.h"
#include "Graphics.h"
#include "GraphicsMath.h"

REGISTER_FINISH(Camera);

Camera::Camera()
{
	name = TYPE_NAME(Camera);
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

bool Camera::Copy(CPScriptBase script)
{
	if (!ScriptBase::Copy(script))
		return false;

	const CPCamera copyScript = dynamic_cast<CPCamera>(script);
	GraphicsMath::ZeroMatrix(&cameraMatrix);

	cameraMatrix = copyScript->cameraMatrix;
	return true;
}

Matrix Camera::GetMatrix(const Matrix& originalMatrix)
{
	if (needRecalculateMatrix)
	{
		needRecalculateMatrix = false;
		GraphicsMath::Inverse(owner->GetWorldMatrix(), cameraMatrix);
		cameraMatrix *= viewMatrix;
	}
	return flipYMatrix * originalMatrix * cameraMatrix;
}

void Camera::Update()
{
	if (_followingObj != nullptr)
	{
		const Vector3 delta = _followingObj->GetWorldTransform() - owner->GetWorldTransform();
		owner->Translate(delta * _dragFactor);
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

void Camera::SetFollow(PGameObj followObj)
{
	_followingObj = followObj;
}

PGameObj Camera::GetFollow()
{
	return _followingObj;
}

void Camera::RemoveFollow()
{
	_followingObj = nullptr;
}
