#pragma once
#include "Node.h"
#include "Vector3.h"
#include "GameObj.h"

class ChasingLeaf : public LeafNode
{
public:
	Node::State Tick() override;
private:
	GameObj* _player = nullptr;

	float _maxDistance;
	Vector3 _direction;
	Vector3 _destination;
};

