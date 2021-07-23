#include "AttackMove.h"
#include "TubeScript.h"
#include "Physic.h"
#include "PhysicCollide.h"
#include "PlayerControl.h"
#include "FieldNames.h"
#include "RigidBody2D.h"
#include "Setting.h"
#include "DebugRenderer.h"
#include "Enemy.h"

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
		Fields::SpecialTag::GetCharacterTag() | Fields::SpecialTag::GetPlayerAttack(),
		PhysicCollide::FilterMode::Collide
	);
	PhysicCollide::GetCollidedWithRectangle(
		GetGameObject(),
		listOfGameObject2,
		_center2,
		_width2,
		_height2,
		Fields::SpecialTag::GetCharacterTag() | Fields::SpecialTag::GetPlayerAttack(),
		PhysicCollide::FilterMode::Collide
	);

	for (auto& package : _packages)
	{
		listOfGameObject1.erase(package._gameObject);
		listOfGameObject2.erase(package._gameObject);
	}

	// To make sure that the player won't just go back in,
	// this will disable one entry until the player no long exists near the tube.
	StopPoint(_stopPoint1TillNoPlayer, listOfGameObject1);
	StopPoint(_stopPoint2TillNoPlayer, listOfGameObject2);

	// Create package for each end
	CreatePackage(true, listOfGameObject1);
	CreatePackage(false, listOfGameObject2);

	TubePackage* package;
	for (auto it = _packages.begin(); it != _packages.end();)
	{
		package = &(*it);
		if (package->_baseComponentPtr.expired())
			it = _packages.erase(it);
		else
		{
			package->_gameObject->GetTransform().Translate(package->_direction * _speed * Physic::GetStep());
			Vector3 deltaFromDes = _path[package->_index] - package->_gameObject->GetTransform().GetWorldTransform();

			if (deltaFromDes.Dot(package->_direction) < 0)
			{
				bool needExit = false;
				// Position correction.
				package->_gameObject->GetTransform().Translate(deltaFromDes);

				if (package->_point1ToPoint2)
				{
					package->_index++;
					if (package->_index == _path.size())
						needExit = true;
					else
						package->_direction = _path[package->_index] - _path[package->_index - 1];
				}
				else
				{
					package->_index--;
					if (package->_index < 0)
						needExit = true;
					else
						package->_direction = _path[package->_index] - _path[static_cast<long long>(package->_index) + 1];
				}

				if (needExit)
				{
					Exit(*package);
					it = _packages.erase(it);
				}
				else
				{
					package->_direction = package->_direction.GetUnitVector();
					++it;
				}
			}
			else
				++it;
		}
	}

	if constexpr (Setting::IsDebugMode())
	{
		for (int i = 0; i < _path.size() - 1; i++)
		{
			DebugRenderer::DrawLine(_path[i], _path[static_cast<long long>(i) + 1], GetWorldMatrix(), DEBUG_RENDER_COLOR);
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

ScriptBase* TubeScript::Clone() const
{
	auto clone = dynamic_cast<TubeScript*>(ScriptBase::Clone());

	clone->_center1 = _center1;
	clone->_width1 = _width1;
	clone->_height1 = _height1;

	clone->_center2 = _center2;
	clone->_width2 = _width2;
	clone->_height2 = _height2;

	clone->_speed = _speed;
	clone->_path = _path;

	return clone;
}

void TubeScript::StopPoint(bool& stopFlag, std::set<GameObj*>& objList)
{
	if (stopFlag)
	{
		stopFlag = false;
		for (auto& obj : objList)
			if (obj->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
			{
				stopFlag = true;
				objList.clear();
				break;
			}
	}
}

void TubeScript::CreatePackage(bool flag, std::set<GameObj*>& objList)
{
	for (auto& obj : objList)
	{
		if (obj->GetPhysicComponent().GetRigidBody2DScript())
		{
			_packages.push_back(TubePackage(obj, flag, _path));
			_packages.back()._rb->Disable();
		}
	}
}

void TubeScript::Exit(TubePackage& package)
{
	if (package._gameObject->GetTag().Collide(Fields::SpecialTag::GetPlayerTag()))
	{
		_isPlayerInTube = false;
		Vector3 vel;
		if (package._point1ToPoint2)
		{
			_stopPoint2TillNoPlayer = true;
			vel = _path.back() - _path[_path.size() - 2];
		}
		else
		{
			_stopPoint1TillNoPlayer = true;
			vel = _path.front() -_path[1];
		}
		package._rb->Enable();
		package._rb->SetVelocity(vel.GetUnitVector() * _speed);

		package._attackScript->Enable();
		package._playerControl->Enable();
		package._playerControl->SetIsInTube(false);
	}
	else if (!_isPlayerInTube)
		package._gameObject->CallDestroy();
}

TubeScript::TubePackage::TubePackage(GameObj* gameObj, bool isPoint1To2, const std::vector<Vector3>& path)
{
	_gameObject = gameObj;
	Vector3 deltaFromDes = _gameObject->GetTransform().GetWorldTransform();
	if (isPoint1To2)
	{
		//front to back
		_index = 1;
		_direction = path[_index] - path.front();
		deltaFromDes = path[0] - deltaFromDes;
	}
	else
	{
		// back to front
		_index = static_cast<int>(path.size()) - 2;
		_direction = path[_index] - path.back();
		deltaFromDes = path.back() - deltaFromDes;
	}
	if (abs(deltaFromDes.x) > abs(deltaFromDes.y))
		deltaFromDes.x = 0;
	else if (abs(deltaFromDes.x) < abs(deltaFromDes.y))
		deltaFromDes.y = 0;
	_gameObject->GetTransform().Translate(deltaFromDes);

	_direction = _direction.GetUnitVector();
	_point1ToPoint2 = isPoint1To2;
	_baseComponentPtr = _gameObject->GetSharedPtr();
	_rb = dynamic_cast<RigidBody2D*>(_gameObject->GetScriptContainer().GetItemType(TYPE_NAME(RigidBody2D)));

	if (_gameObject->GetTag().Contain(Fields::SpecialTag::GetPlayerTag()))
	{
		_attackScript = _gameObject->GetScriptContainer().GetItemType(TYPE_NAME(AttackMove));
		_playerControl = dynamic_cast<PlayerControl*>(_gameObject->GetScriptContainer().GetItemType(TYPE_NAME(PlayerControl)));
		_playerControl->SetIsInTube(true);
		_attackScript->Disable();
		_playerControl->Disable();
	}
	else
	{
		_gameObject->GetChildContainer().IteratingWithLamda([](PGameObj obj) {obj->Disable(); });
		_gameObject->GetScriptContainer().IteratingWithLamda([](PScriptBase script) {
			if (script->GetType() == TYPE_NAME(Polygon2DCollider))
				script->Disable();
			});
		if (_gameObject->GetTag().Contain(Fields::SpecialTag::GetEnemyTag()))
		{
			auto enemyScript = dynamic_cast<Enemy*>(_gameObject->GetScriptContainer().GetItemType(TYPE_NAME(Enemy)));
			enemyScript->SetIsInTube();
		}
	}
}
