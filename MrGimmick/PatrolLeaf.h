#pragma once
#include "Node.h"
#include "Vector3.h"
#include <vector>

class PatrolLeaf : public LeafNode
{
public:
	Node::State Tick() override;
	void CustomLoad(nlohmann::json& input) override;
private:
	std::vector<Vector3> _path;
	Vector3 _direction;
	unsigned _currentDes = -1;

	float _delayCounter = 0;
	float _delay = 0;

	bool _walking = false;

	static constexpr auto _pathField = "Path";
	static constexpr auto _delayField = "Delay";
	NODE_REGISTER(PatrolLeaf);
};

