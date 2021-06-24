#include "AnimationTree.h"

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
