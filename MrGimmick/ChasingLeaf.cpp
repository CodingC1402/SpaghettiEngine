#include "ChasingLeaf.h"
#include "PlayerScript.h"
#include "AIBTs.h"

Node::State ChasingLeaf::Tick()
{
    SAIBTs tree = std::dynamic_pointer_cast<AIBTs>(_tree.lock());
    if (!_player)
    {
        _player = PlayerScript::GetCurrentPlayer();
        if (!_player)
            return Node::State::Failure;

        _destination = _player->GetTransform().GetWorldTransform();
        _direction = _destination - tree->GetGameObject()->GetTransform().GetWorldTransform();
        if (abs(_direction.x) < 4.0f)
        {
            tree->StopMove();
            return State::Success;
        }

        float distance = _direction.GetMagnitude();
        if (distance > _maxDistance)
            _destination /= (distance / _maxDistance);

        if (_direction.x > 0)
            tree->MoveRight();
        else if (_direction.x < 0)
            tree->MoveLeft();

        if (distance < _range)
        {
            tree->StopMove();
            return State::Success;
        }


        return State::Running;
    }

    auto pos = tree->GetGameObject()->GetTransform().GetWorldTransform();
    auto delta = _destination - pos;

    if (auto distance = (_player->GetTransform().GetWorldTransform() - pos).GetMagnitude(); distance > _scanRadius)
        return State::Failure;

    _timeOutAccumulative += tree->GetDeltaTime();
    if (delta.x * _direction.x < 0 || _timeOutAccumulative > _timeOut)
    {
        _timeOutAccumulative = 0.0f;
        _player = nullptr;
        tree->StopMove();
    }
    return State::Running;
}

void ChasingLeaf::CustomLoad(nlohmann::json& input)
{
    _maxDistance = input[_maxDistanceField].get<float>();
    _maxStuck = input[_maxStuckField].get<unsigned>();
    _scanRadius = input[_radiusField].get<float>();
    _timeOut = input[_timeOutField].get<float>();
    if (input[_rangeField].empty())
        _range = 0;
    else
        _range = input[_rangeField].get<float>();
}

SNode ChasingLeaf::Clone(WBTs tree)
{
    auto clone = std::dynamic_pointer_cast<ChasingLeaf>(Node::Clone(tree));

    clone->_maxStuck        = _maxStuck;
    clone->_stuckCounter    = _stuckCounter;
    clone->_maxDistance     = _maxDistance;
    clone->_scanRadius      = _scanRadius;
    clone->_direction       = _direction;
    clone->_destination     = _destination;
    clone->_timeOut         = _timeOut;
    clone->_oldPos          = _oldPos;
    clone->_player          = _player;
    clone->_range           = _range;

    return clone;
}
