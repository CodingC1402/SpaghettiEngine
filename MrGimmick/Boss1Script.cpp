#include "Boss1Script.h"
#include "LoadingJson.h"
#include "GameTimer.h"
#include "PlayerScript.h"
#include "SMath.h"

REGISTER_FINISH(Boss1Script, ScriptBase) {}

void Boss1Script::OnStart()
{
	auto aiScript = GET_FIRST_SCRIPT_OF_TYPE(AIScript);
	_readyAttack = aiScript->GetBTS()->GetBlackBoard().lock()->GetField<bool>("ReadyAttack");
	_moveScript = GET_FIRST_SCRIPT_OF_TYPE(MoveScript);
	_rb = GET_FIRST_SCRIPT_OF_TYPE(RigidBody2D);
	_healthScript = GET_FIRST_SCRIPT_OF_TYPE(HealthScript);
	_healthScript->AddToHealthEvent(
		[&](const int& health, const int&) {
			if (health <= 0)
			{
				GetOwner()->Instantiate(_loadNextScenePrefab, Vector3(0, 0, 0));
			}
		}
	);

	_force = Vector3(70, 100, 0);

	_flipXMatrix = Matrix4::GetDiagonalMatrix();
	_flipXMatrix._11 = -1;
	_flipForceMatrix = _flipXMatrix;
	_flipForceMatrix._22 = -1;
}

void Boss1Script::OnUpdate()
{
	if (_shotCounter == 0 && _readyAttack.lock()->GetValue())
	{
		_shotCounter = _count;

		_currentForceVec = _force;
	}

	if (_shotCounter > 0)
	{
		if (_delayCounter > 0)
		{
			_delayCounter -= GameTimer::GetDeltaTime();
		}
		else
		{
			for (auto it = _created.begin(); it != _created.end();)
			{ 
				if ((*it).expired())
					it = _created.erase(it);
				else
					++it;
			}
			if (_created.size() > 8)
				return;

			_delayCounter = _delay;
			auto ran = std::rand() % 100;
			GameObj* spawnPrefab = nullptr;
			if (ran >= 95)
				spawnPrefab = _electricPrefab;
			else
				spawnPrefab = _normalEnemyPrefab;


			auto spawned = GetOwner()->Instantiate(spawnPrefab, GetWorldTransform());
			auto rb = dynamic_cast<RigidBody2D*>(spawned->GetScriptContainer().GetItemType(TYPE_NAME(RigidBody2D)));
			if (_moveScript->IsFlipped())
			{
				rb->SetVelocity(_currentForceVec * _flipXMatrix);
				_rb->SetVelocity(_currentForceVec * _flipForceMatrix * _flipXMatrix);
			}
			else
			{
				rb->SetVelocity(_currentForceVec);
				_rb->SetVelocity(_currentForceVec * _flipForceMatrix);
			}
			_shotCounter--;
			_created.push_back(std::dynamic_pointer_cast<GameObj>(spawned->GetSharedPtr()));
		}

		if (_shotCounter == 0)
		{
			_readyAttack.lock()->SetValue(false);
			_delayCounter = 0;
		}
	}
}

void Boss1Script::OnDisabled()
{
	for (auto& obj : _created)
	{
		if (!obj.expired())
			obj.lock()->CallDestroy();
	}
	_created.clear();
	_healthScript->SetHealth(_healthScript->GetMaxHealth());
}

ScriptBase* Boss1Script::Clone() const
{
	auto clone = dynamic_cast<Boss1Script*>(ScriptBase::Clone());

	clone->_normalEnemyPrefab = _normalEnemyPrefab;
	clone->_electricPrefab = _electricPrefab;
	clone->_loadNextScenePrefab = _loadNextScenePrefab;

	return clone;
}

void Boss1Script::Load(nlohmann::json& input)
{
	_normalEnemyPrefab = GET_REF_OBJECT(0);
	_electricPrefab =	GET_REF_OBJECT(1);
	_loadNextScenePrefab =	GET_REF_OBJECT(2);
}
