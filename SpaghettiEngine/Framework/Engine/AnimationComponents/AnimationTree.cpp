#include "AnimationTree.h"
#include "GameObj.h"

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

GameObj* AnimationTree::GetOwner() const noexcept
{
	return _ownerObj;
}

Color AnimationTree::GetColor() const noexcept
{
	return _color;
}

void AnimationTree::SetCurrentSprite(SSprite sprite)
{
	_currentSprite = sprite;
}

void AnimationTree::SetOwner(GameObj* owner) noexcept
{
	_ownerObj = owner;
}

void AnimationTree::SetColor(const Color& color) noexcept
{
	_color = color;
}
