#pragma once
#include "ScriptBase.h"
#include "PlayerControl.h"
#include <vector>

class TubeScript : public ScriptBase
{
private:
	struct TubePackage
	{
	public:
		TubePackage() = default;
		TubePackage(GameObj* gameObj, bool isPoint1To2, const std::vector<Vector3>& path);
	public:
		bool _point1ToPoint2 = false;
		int	_index			 = 0;
		Vector3	_direction;
		GameObj* _gameObject = nullptr;
		WBaseComponent _baseComponentPtr;
		RigidBody2D* _rb	 = nullptr;
		ScriptBase* _attackScript = nullptr;
		PlayerControl* _playerControl = nullptr;
	};
public:
	void OnFixedUpdate();
	void Load(nlohmann::json& input) override;
	ScriptBase* Clone() const override;
private:
	void StopPoint(bool& stopFlag, std::set<GameObj*>& objList);
	void CreatePackage(bool flag, std::set<GameObj*>& objList);
	void Exit(TubePackage& package);
private:
	Vector3 _center1;
	float _width1 = 0;
	float _height1 = 0;

	Vector3 _center2;
	float _width2 = 0;
	float _height2 = 0;

	bool _stopPoint1TillNoPlayer = false;
	bool _stopPoint2TillNoPlayer = false;

	bool _isPlayerInTube;
	float _speed = 0;
	std::vector<Vector3> _path;
	std::list<TubePackage> _packages;

	REGISTER_START(TubeScript);
};

