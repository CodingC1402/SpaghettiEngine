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
	~Camera();
	virtual void Start() override;
	virtual bool Copy(const PScriptBase script) override;
	virtual PMatrix GetMatrix();
	virtual PMatrix GetScreenMatrix();
	virtual void OnDisabled() override;
	virtual void OnEnabled() override;
protected:
	virtual void Load(const std::string* inputArg, int argS) override;
protected:
	PMatrix cameraMatrix;
	PMatrix screenMatrix;
private:
	REGISTER_START(Camera);
};
