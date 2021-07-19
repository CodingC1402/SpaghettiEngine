#include "SearchingLeaf.h"
#include "GameObj.h"
#include "FieldNames.h"
#include "PlayerScript.h"
#include "AIBTs.h"

Node::State SearchingLeaf::Tick()
{
	auto player = PlayerScript::GetCurrentPlayer();
	if (!player)
		return State::Failure;

	SAIBTs tree = std::dynamic_pointer_cast<AIBTs>(_tree.lock());
	float distance = (player->GetTransform().GetWorldTransform() - tree->GetGameObject()->GetTransform().GetWorldTransform()).GetMagnitude();
	if (distance > _scanRadius)
		return State::Failure;
	else
		return State::Success;
}

void SearchingLeaf::CustomLoad(nlohmann::json& input)
{
	_scanRadius = input[SearchingLeaf::_radiusField].get<float>();
}

SNode SearchingLeaf::Clone(WBTs tree)
{
	auto clone = std::dynamic_pointer_cast<SearchingLeaf>(LeafNode::Clone(tree));

	clone->_scanRadius = _scanRadius;

	return clone;
}
