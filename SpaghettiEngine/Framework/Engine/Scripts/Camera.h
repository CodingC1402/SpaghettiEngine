#pragma once
#include "ScriptBase.h"

typedef class Camera* PCamera;
typedef const Camera* CPCamera;

class Camera : public ScriptBase
{
	friend class Graphics;
public:
	virtual Matrix4	GetMatrix(const Matrix4& originalMatrix);
	virtual Matrix4 GetMatrixWithoutScaleY(const Matrix4& originalMatrix);
	virtual void	OnFixedUpdate() override;

	virtual void	OnDisabled() override;
	virtual void	OnEnabled() override;

	virtual void	Load(json& input) override;

	virtual void	SetFollow(PGameObj followObj);
	[[nodiscard]] virtual PGameObj	GetFollow();
	virtual void	RemoveFollow();

	PScriptBase Clone() const override;
protected:
	Matrix4 cameraMatrix;
	Matrix4 viewMatrix;
	Matrix4 flipYMatrix;
	
	WGameObj _followingPtr;
	
	int oldW = 0;
	int oldH = 0;
	bool needRecalculateMatrix = true;
private:
	REGISTER_START(Camera);
};
