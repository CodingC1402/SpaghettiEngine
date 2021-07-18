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

        float distance = _direction.GetMagnitude();
        if (distance > _maxDistance)
            _destination /= (distance / _maxDistance);

        if (_direction.x > 0)
            tree->MoveRight();
        else if (_direction.x < 0)
            tree->MoveLeft();

        return State::Running;
    }

    auto delta = _destination - tree->GetGameObject()->GetTransform().GetWorldTransform();
    if (delta.Dot(_direction) < 0)
    {
        _player = nullptr;
        tree->StopMove();
        return State::Success;
    }
}
