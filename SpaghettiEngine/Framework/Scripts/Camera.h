#pragma once
#include "ScriptBase.h"
#include "CornDirectX.h"

typedef class Camera* PCamera;
typedef const Camera* CPCamera;

class Camera : public ScriptBase
{
	friend class Graphics;
public:
	Camera(PScene owner);
	virtual Matrix	GetMatrix(const Matrix& originalMatrix);
	virtual void	OnUpdate() override;

	virtual void	OnDisabled() override;
	virtual void	OnEnabled() override;
	virtual BaseComponent*	Clone() override;

	virtual void	SetFollow(PGameObj followObj);
	virtual [[nodiscard]] PGameObj	GetFollow();
	virtual void	RemoveFollow();
protected:
	Matrix cameraMatrix;
	Matrix viewMatrix;
	Matrix flipYMatrix;
	
	Scene::WBaseComponent _followingPtr;
	PGameObj _followingObj;
	float _dragFactor = 0.7f;
	
	int oldW = 0;
	int oldH = 0;
	bool needRecalculateMatrix = true;
private:
	REGISTER_START(Camera);
};
