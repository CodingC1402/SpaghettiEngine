#include "TubeScript.h"
#include "Physic.h"
#include "PhysicCollide.h"
#include "FieldNames.h"
#include "RigidBody2D.h"

void TubeScript::OnFixedUpdate()
{
	std::set<GameObj*> listOfGameObject1;
	std::set<GameObj*> listOfGameObject2;

	PhysicCollide::GetCollidedWithRectangle(
		GetGameObject(),
		listOfGameObject1,
		_center1,
		_width1,
		_height1,
		Fields::SpecialTag::GetStarOrCharacterTag(),
		PhysicCollide::FilterMode::Collide
	);
	PhysicCollide::GetCollidedWithRectangle(
		GetGameObject(),
		listOfGameObject2,
		_center2,
		_width2,
		_height2,
		Fields::SpecialTag::GetStarOrCharacterTag(),
		PhysicCollide::FilterMode::Collide
	);

	for (auto& package : _packages)
	{
		listOfGameObject1.erase(package._gameObject);
		listOfGameObject2.erase(package._gameObject);
	}

	// To make sure that the player won't just go back in,
	// this will disable one entry until the player no long exists near the tube.
	if (_stopPoint1TillNoPlayer)
	{
		_stopPoint1TillNoPlayer = false;
		for (auto& obj : listOfGameObject1)
			if (obj->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
				_stopPoint1TillNoPlayer = true;
	}
	if (_stopPoint2TillNoPlayer)
	{
		_stopPoint2TillNoPlayer = false;
		for (auto& obj : listOfGameObject2)
			if (obj->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
				_stopPoint2TillNoPlayer = true;
	}
	
	for (auto& obj : listOfGameObject1)
		_packages.push_back(TubePackage(obj, true, _path));

	for (auto& obj : listOfGameObject2)
		_packages.push_back(TubePackage(obj, false, _path));

	for (auto it = _packages.begin(); it != _packages.end();)
	{
		(*it)._gameObject->GetTransform().Translate((*it)._direction * _speed * Physic::GetStep());
		Vector3 deltaFromDes = _path[(*it)._index] - (*it)._gameObject->GetTransform().GetWorldTransform();

		if (deltaFromDes.Dot((*it)._direction) < 0)
		{
			if ((*it)._point1ToPoint2)
			{
				(*it)._index++;
				(*it)._direction = _path[(*it)._index] - _path[(*it)._index - 1];
			}
			else
			{
				(*it)._index--;
				(*it)._direction = _path[(*it)._index] - _path[static_cast<long long>((*it)._index) + 1];
			}

			if ((*it)._index == 0 || (*it)._index == _path.size() - 1)
			{
				Exit((*it));
				it = _packages.erase(it);
			}
			else
				++it;
		}
		else
			++it;
	}
}

void TubeScript::Load(nlohmann::json& input)
{
	_center1 = input[Fields::TubeScript::_pointA][Fields::TubeScript::_center];
	_width1  = input[Fields::TubeScript::_pointA][Fields::TubeScript::_width].get<float>();
	_height1 = input[Fields::TubeScript::_pointA][Fields::TubeScript::_height].get<float>();

	_center2 = input[Fields::TubeScript::_pointB][Fields::TubeScript::_center];
	_width2  = input[Fields::TubeScript::_pointB][Fields::TubeScript::_width].get<float>();
	_height2 = input[Fields::TubeScript::_pointB][Fields::TubeScript::_height].get<float>();
}

void TubeScript::Exit(TubePackage& package)
{
	if (package._gameObject->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		_isPlayerInTube = false;
		if (package._point1ToPoint2)
			_stopPoint2TillNoPlayer = true;
		else
			_stopPoint1TillNoPlayer = true;
	}
	else if (!_isPlayerInTube)
		package._gameObject->CallDestroy();
}

TubeScript::TubePackage::~TubePackage()
{
	_rb->Enable();
}

TubeScript::TubePackage::TubePackage(GameObj* gameObj, bool isPoint1To2, const std::vector<Vector3>& path)
{
	if (isPoint1To2)
	{
		//front to back
		_direction = path[1] - path.front();
		_index = 1;
	}
	else
	{
		// back to front
		_direction = path[path.size() - 2] - path.back();
		_index = path.size() - 2;
	}
	_point1ToPoint2 = isPoint1To2;
	_gameObject = gameObj;
	_rb = _gameObject->GetScriptContainer().GetItemType(TYPE_NAME(RigidBody2D));
	_rb->Disable();
}
