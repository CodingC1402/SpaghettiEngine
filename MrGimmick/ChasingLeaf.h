#pragma once
#include "Node.h"
#include "Vector3.h"
#include "GameObj.h"

class ChasingLeaf : public LeafNode
{
public:
	Node::State Tick() override;
	void CustomLoad(nlohmann::json& input) override;
	SNode Clone(WBTs tree) override;
private:
	GameObj* _player = nullptr;

	unsigned _stuckCounter;
	unsigned _maxStuck;
	float _maxDistance = 0;
	float _scanRadius = 0;

	float _timeOut = 0;
	float _timeOutAccumulative = 0;

	Vector3 _direction;
	Vector3 _destination;
	Vector3 _oldPos;

	static constexpr auto _radiusField = "Radius";
	static constexpr auto _maxStuckField = "MaxStuck";
	static constexpr auto _maxDistanceField = "MaxDistance";
	static constexpr auto _timeOutField = "TimeOut";
	NODE_REGISTER(ChasingLeaf);
};

