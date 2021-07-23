#pragma once

#include "BTs.h"
#include "Animation.h"
#include "Macros.h"
#include "Color.h"

CLASS_FORWARD_DECLARATION(AnimationTree);
class GameObj;

class AnimationTree : public BTs
{
	friend class AnimationNode;
	friend class Animator;
	friend class SkeletonNode;
public:
	[[nodiscard]] static SAnimationTree CreateAnimationTree();

	[[nodiscard]] SBTs Clone() override;
	[[nodiscard]] SSprite GetCurrentSprite() const;
	[[nodiscard]] GameObj* GetOwner() const noexcept;
	[[nodiscard]] Color GetColor() const noexcept;
protected:
	void SetCurrentSprite(SSprite sprite);
	void SetOwner(GameObj* owner) noexcept;
	void SetColor(const Color& color) noexcept;
protected:
	SSprite _currentSprite;
	GameObj* _ownerObj;
	Color _color = 0xFFFFFFFF;
private:
	REGISTER_BEHAVIOR_TREE_START(AnimationTree);
};