#include "AnimationNode.h"
#include "LoadingJson.h"
#include "GameTimer.h"
#include "AnimationTree.h"
#include "SMath.h"

Node::State AnimationNode::Tick()
{
    _time += GameTimer::GetDeltaTime();

    unsigned indexBefore = _index;
    bool ended = _anim->Advance(_index, _time);
    
    auto animTree = std::dynamic_pointer_cast<AnimationTree>(_tree.lock());
    animTree->SetCurrentSprite(_anim->GetSpriteOfFrame(_index));

    if (ended)
    {
        return Node::State::Success;
        Reset();
    }
    else
        return Node::State::Running;
}

void AnimationNode::OnInterrupted()
{
    LeafNode::OnInterrupted();
    Reset();
}

void AnimationNode::CustomLoad(nlohmann::json& input)
{
    using LoadingJson::Field;
    _anim = AnimationContainer::GetInstance()->GetResource(input[Field::idField].get<CULL>());
}

void AnimationNode::Reset()
{
    _time = 0.0f;
    _index = 0u;
}

SNode AnimationNode::Clone(WBTs tree)
{
    auto clone = std::dynamic_pointer_cast<AnimationNode>(Node::Clone(tree));

    clone->_anim = _anim;

    return clone;
}
