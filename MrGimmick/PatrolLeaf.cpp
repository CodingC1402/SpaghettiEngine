#include "PatrolLeaf.h"
#include "AIBTs.h"
#include "SMath.h"

// Only care about x axis.
// May produce some error when used in the surface that have incline.

Node::State PatrolLeaf::Tick()
{
    SAIBTs tree = std::dynamic_pointer_cast<AIBTs>(_tree.lock());
    if (!_walking)
    {
        // Loop to find the next position, if the position is the same as the current
        // pos then it will skip.
        _delayCounter += tree->GetDeltaTime();
        if (_delayCounter < _delay)
            return State::Failure;
        else
            _delayCounter = 0;

        while (SMath::CompareFloat(_direction.x, 0))
        {
            _currentDes++;
            _currentDes = _currentDes % _path.size();
            _direction = _path[_currentDes] - tree->GetGameObject()->GetTransform().GetWorldTransform();
            if (_direction.x > 0)
                tree->MoveRight();
            else if (_direction.x < 0)
                tree->MoveLeft();
        }
        _walking = true;
    }
    else
    {
        auto delta = _path[_currentDes] - tree->GetGameObject()->GetTransform().GetWorldTransform();
        if (delta.x * _direction.x < 0)
        {
            _walking = false;
            tree->StopMove();
            _direction.x = 0;
            return State::Success;
        }
    }
    return State::Running;
}

void PatrolLeaf::CustomLoad(nlohmann::json& input)
{
    _delay = input[_delayField].get<float>();
    for (auto& pos : input[_pathField])
    {
        _path.push_back(pos);
    }
}

SNode PatrolLeaf::Clone(WBTs tree)
{
    auto clone = std::dynamic_pointer_cast<PatrolLeaf>(LeafNode::Clone(tree));

    clone->_path = _path;
    clone->_delay = _delay;

    return clone;
}
