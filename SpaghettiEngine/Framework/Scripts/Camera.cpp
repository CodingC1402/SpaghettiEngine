#include "Camera.h"
#include "CornException.h"
#include "Setting.h"
#include "CornException.h"
#include "GraphicsMath.h"

REGISTER_FINISH(Camera);

Camera::Camera()
{
	name = TYPE_NAME(Camera);

	cameraMatrix = GraphicsMath::NewMatrix();
	cameraMatrix->_11 = 1;
	cameraMatrix->_22 = -1;
	cameraMatrix->_33 = 1;
	cameraMatrix->_44 = 1;
}

Camera::~Camera()
{
	if (cameraMatrix)
	{
		delete cameraMatrix;
		cameraMatrix = NULL;
	}
}

bool Camera::Copy(const PScriptBase script)
{
	if (!ScriptBase::Copy(script))
		return false;

	PCamera copyScript = static_cast<PCamera>(script);
	if (cameraMatrix)
		delete cameraMatrix;

	cameraMatrix = GraphicsMath::NewMatrix();

	*cameraMatrix = *copyScript->cameraMatrix;
	return true;
}

void Camera::Start()
{
	if (!isDisabled)
		Graphics::AddCamera(this);
}

const PMatrix Camera::GetMatrix()
{
	Size resolution = Setting::GetResolution();
	Vector3 ownerPos = *owner->GetPosition();
	cameraMatrix->_41 = -(ownerPos.x - resolution.width / 2.0f);
	cameraMatrix->_42 = +(ownerPos.y + resolution.height / 2.0f);
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
