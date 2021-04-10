#include "Camera.h"
#include "CornException.h"
#include "Setting.h"

REGISTER_FINISH(Camera);

Camera::Camera()
{
	name = TYPE_NAME(Camera);

	cameraMatrix = new Matrix();
	cameraMatrix->_11 = 1;
	cameraMatrix->_22 = -1;
	cameraMatrix->_33 = 1;
	cameraMatrix->_44 = 1;

	screenMatrix = NULL;
}

Camera::~Camera()
{
	if (cameraMatrix)
	{
		delete cameraMatrix;
		cameraMatrix = NULL;
	}
	if (screenMatrix)
	{
		delete screenMatrix;
		screenMatrix = NULL;
	}
}

bool Camera::Copy(const PScriptBase script)
{
	if (!ScriptBase::Copy(script))
		return false;

	PCamera copyScript = static_cast<PCamera>(script);
	if (cameraMatrix)
		delete cameraMatrix;
	if (screenMatrix)
		delete screenMatrix;

	if (copyScript->screenMatrix)
	{
		screenMatrix = new Matrix();
		*screenMatrix = *copyScript->screenMatrix;
	}
	else
		screenMatrix = NULL;

	cameraMatrix = new Matrix();

	cameraMatrix->_11 = 1;
	cameraMatrix->_22 = -1;
	cameraMatrix->_33 = 1;
	cameraMatrix->_44 = 1;

	*cameraMatrix = *copyScript->cameraMatrix;
	return true;
}

void Camera::Start()
{
	if (!isDisabled)
		Graphics::AddCamera(this);
}

PMatrix Camera::GetMatrix()
{
	Size resolution = Setting::GetResolution();
	Vector3 ownerPos = owner->GetPosition();
	cameraMatrix->_41 = -(ownerPos.x - resolution.width / 2.0);
	cameraMatrix->_42 = -(ownerPos.y + resolution.height / 2.0);
	return cameraMatrix;
}

PMatrix Camera::GetScreenMatrix()
{
	return screenMatrix;
}

void Camera::OnDisabled()
{
	Graphics::RemoveCamera(this);
}

void Camera::OnEnabled()
{
	Graphics::AddCamera(this);
}

void Camera::Load(const std::string* inputArg, int argS)
{
	if (!argS)
		return;
	screenMatrix = new Matrix();
	screenMatrix->_41 = std::stoi(inputArg[0]);
	screenMatrix->_42 = std::stoi(inputArg[1]);
	screenMatrix->_11 = std::stof(inputArg[2]);
	screenMatrix->_22 = std::stof(inputArg[3]);
}
