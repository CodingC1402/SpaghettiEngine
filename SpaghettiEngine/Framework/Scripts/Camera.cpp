#include "Camera.h"
#include "CornException.h"
#include "Setting.h"
#include "CornException.h"
#include "GraphicsMath.h"

REGISTER_FINISH(Camera);

Camera::Camera()
{
	name = TYPE_NAME(Camera);

	GraphicsMath::ZeroMatrix(&cameraMatrix);
	cameraMatrix._11 = 1;
	cameraMatrix._22 = -1;
	cameraMatrix._33 = 1;
	cameraMatrix._44 = 1;
}

bool Camera::Copy(const PScriptBase script)
{
	if (!ScriptBase::Copy(script))
		return false;

	const PCamera copyScript = dynamic_cast<PCamera>(script);
	GraphicsMath::ZeroMatrix(&cameraMatrix);

	cameraMatrix = copyScript->cameraMatrix;
	return true;
}

void Camera::Start()
{
	if (!isDisabled)
		Graphics::AddCamera(this);
}

Matrix Camera::GetMatrix()
{
	cameraMatrix = owner->GetWorldMatrix();
	const Size resolution = Setting::GetResolution();
	cameraMatrix._41 = -(static_cast<float>(resolution.width) / 2.0f);
	cameraMatrix._42 = +(static_cast<float>(resolution.height) / 2.0f);
	return cameraMatrix;
}

void Camera::OnDisabled()
{
	Graphics::RemoveCamera(this);
}

void Camera::OnEnabled()
{
	Graphics::AddCamera(this);
}

void Camera::Unload()
{
	Graphics::RemoveCamera(this);
}
