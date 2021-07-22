#pragma once
#include "ScriptBase.h"
#include "CRectangle.h"

class CameraBoundingBox : public ScriptBase
{
public:
	void OnFixedUpdate() override;
	void OnLateUpdate() override;

	void OnDisabled() override;
	void Load(nlohmann::json& input) override;

	static void TempShutDown();
	static void TurnOn();
private:
	// Trigger area
	std::vector<CRectangle> _triggerZones;
	// Bounding box
	float _minX = 0.0f;
	float _maxX = 0.0f;
	float _minY = 0.0f;
	float _maxY = 0.0f;
	// Use to make sure that the script don't switch on and off when collide with player at the same time
	bool _isPlayerInside = false;
	
	static bool __tempDisable;
	static CameraBoundingBox* __currentScript;
	REGISTER_START(CameraBoundingBox);
};

