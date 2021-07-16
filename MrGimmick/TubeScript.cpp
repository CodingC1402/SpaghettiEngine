#include "TubeScript.h"
#include "Physic.h"
#include "PhysicCollide.h"
#include "FieldNames.h"
#include "RigidBody2D.h"
#include "Setting.h"
#include "DebugRenderer.h"

static constexpr unsigned DEBUG_RENDER_COLOR = 0xFFFF0077;
REGISTER_FINISH(TubeScript, ScriptBase) {}

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
		Fields::SpecialTag::GetPlayerTag(),
		PhysicCollide::FilterMode::Collide
	);
	PhysicCollide::GetCollidedWithRectangle(
		GetGameObject(),
		listOfGameObject2,
		_center2,
		_width2,
		_height2,
		Fields::SpecialTag::GetCharacterTag(),
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
			{
				_stopPoint1TillNoPlayer = true;
				listOfGameObject1.clear();
				break;
			}
	}
	if (_stopPoint2TillNoPlayer)
	{
		_stopPoint2TillNoPlayer = false;
		for (auto& obj : listOfGameObject2)
			if (obj->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
			{
				_stopPoint2TillNoPlayer = true;
				listOfGameObject2.clear();
				break;
			}
	}
	else
	{
		int i = 100;
	}
	
	for (auto& obj : listOfGameObject1)
	{
		_packages.push_back(TubePackage(obj, true, _path));
		_packages.back()._rb->Disable();
	}

	for (auto& obj : listOfGameObject2)
	{
		_packages.push_back(TubePackage(obj, false, _path));
		_packages.back()._rb->Disable();
	}

	for (auto it = _packages.begin(); it != _packages.end();)
	{
		(*it)._gameObject->GetTransform().Translate((*it)._direction * _speed * Physic::GetStep());
		Vector3 deltaFromDes = _path[(*it)._index] - (*it)._gameObject->GetTransform().GetWorldTransform();

		if (deltaFromDes.Dot((*it)._direction) < 0)
		{
			bool needExit = false;
			// Position correction.
			(*it)._gameObject->GetTransform().Translate(deltaFromDes);

			if ((*it)._point1ToPoint2)
			{
				(*it)._index++;
				if ((*it)._index == _path.size())
					needExit = true;
				else
					(*it)._direction = _path[(*it)._index] - _path[(*it)._index - 1];
			}
			else
			{
				(*it)._index--;
				if ((*it)._index < 0)
					needExit = true;
				else
					(*it)._direction = _path[(*it)._index] - _path[static_cast<long long>((*it)._index) + 1];
			}

			if (needExit)
			{
				Exit((*it));
				it = _packages.erase(it);
			}
			else
			{
				(*it)._direction = (*it)._direction.GetUnitVector();
				++it;
			}
		}
		else
			++it;
	}

	if constexpr (Setting::IsDebugMode())
	{
		for (int i = 0; i < _path.size() - 1; i++)
		{
			DebugRenderer::DrawLine(_path[i], _path[i + 1], GetWorldMatrix(), DEBUG_RENDER_COLOR);
		}

		DebugRenderer::DrawRectangle(_center1, _width1, _height1, GetWorldMatrix(), DEBUG_RENDER_COLOR);
		DebugRenderer::DrawRectangle(_center2, _width2, _height2, GetWorldMatrix(), DEBUG_RENDER_COLOR);
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

	_speed = input[Fields::TubeScript::_speed].get<float>();

	for (int i = 0; i < input[Fields::TubeScript::_path].size(); i += 2)
	{
		_path.push_back(Vector3(
			input[Fields::TubeScript::_path][i],
			input[Fields::TubeScript::_path][i + 1],
			0));
	}
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
		package._rb->Enable();
	}
	else if (!_isPlayerInTube)
		package._gameObject->CallDestroy();
}

TubeScript::TubePackage::TubePackage(GameObj* gameObj, bool isPoint1To2, const std::vector<Vector3>& path)
{
	_gameObject = gameObj;
	Vector3 deltaFromDes;

	if (isPoint1To2)
	{
		//front to back
		_direction = path[1] - path.front();
		_index = 1;
		deltaFromDes = path.front() - _gameObject->GetTransform().GetWorldTransform();
	}
	else
	{
		// back to front
		_direction = path[path.size() - 2] - path.back();
		_index = path.size() - 2;
		deltaFromDes = path.back() - _gameObject->GetTransform().GetWorldTransform();
	}

	_direction = _direction.GetUnitVector();
	_point1ToPoint2 = isPoint1To2;
	_gameObject->GetTransform().Translate(deltaFromDes);
	_rb = dynamic_cast<RigidBody2D*>(_gameObject->GetScriptContainer().GetItemType(TYPE_NAME(RigidBody2D)));
	_rb->SetVelocity(Vector3(0, 0, 0));
}
