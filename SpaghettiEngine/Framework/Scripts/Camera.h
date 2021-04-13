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
	virtual const PMatrix GetMatrix();
	virtual void OnDisabled() override;
	virtual void OnEnabled() override;
	virtual void Unload() override;
protected:
	PMatrix cameraMatrix;
private:
	REGISTER_START(Camera);
};
