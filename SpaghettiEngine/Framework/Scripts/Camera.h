#pragma once
#include "ScriptBase.h"
#include "Graphics.h"
#include "CornDirectX.h"

class Graphics;
typedef class Camera* PCamera;

class Camera : public ScriptBase
{
	friend class Graphics;
public:
	Camera();
	virtual void	Start() override;
	virtual bool	Copy(const PScriptBase script) override;
	virtual Matrix	GetMatrix();
	virtual void	OnDisabled() override;
	virtual void	OnEnabled() override;
	virtual void	Unload() override;
protected:
	Matrix cameraMatrix;
private:
	REGISTER_START(Camera);
};
