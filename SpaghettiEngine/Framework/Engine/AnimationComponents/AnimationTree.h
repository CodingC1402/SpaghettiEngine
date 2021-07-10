#pragma once

#include "BTs.h"
#include "Animation.h"
#include "Macros.h"

CLASS_FORWARD_DECLARATION(AnimationTree);

class AnimationTree : public BTs
{
	friend class AnimationNode;
	friend class Animator;
public:
	[[nodiscard]] static SAnimationTree CreateAnimationTree();

	[[nodiscard]] SBTs Clone() override;
	[[nodiscard]] SSprite GetCurrentSprite() const;
protected:
	void SetCurrentSprite(SSprite sprite);
protected:
	SSprite _currentSprite;
private:
	REGISTER_BEHAVIOR_TREE_START(AnimationTree);
};