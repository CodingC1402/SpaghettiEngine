#include "AnimationTree.h"

REGISTER_BEHAVIOR_TREE_FINISH(AnimationTree);

SAnimationTree AnimationTree::CreateAnimationTree()
{
	SAnimationTree newTree = std::make_shared<AnimationTree>();
	newTree->AssignPtr(newTree);
	return newTree;
}

SBTs AnimationTree::Clone()
{
	auto clone = std::dynamic_pointer_cast<AnimationTree>(BTs::Clone());

	clone->_currentSprite = _currentSprite;

	return clone;
}

SSprite AnimationTree::GetCurrentSprite() const
{
	return _currentSprite;
}

void AnimationTree::SetCurrentSprite(SSprite sprite)
{
	_currentSprite = sprite;
}
