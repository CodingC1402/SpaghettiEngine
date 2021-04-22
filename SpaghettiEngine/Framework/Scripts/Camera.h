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
	virtual Matrix	GetMatrix(const Matrix& originalMatrix);
	virtual void	Update() override;
	virtual void	Start() override;
	virtual bool	Copy(const PScriptBase script) override;
	virtual void	OnDisabled() override;
	virtual void	OnEnabled() override;
	virtual void	Unload() override;
protected:
	Matrix cameraMatrix;
	Matrix viewMatrix;
	Matrix flipYMatrix;
	
	int oldW = 0;
	int oldH = 0;
	bool needRecalculateMatrix = true;
private:
	REGISTER_START(Camera);
};
