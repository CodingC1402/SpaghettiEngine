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
	[[nodiscard]] SBTs Clone() override;
	[[nodiscard]] SSprite GetCurrentSprite() const;
protected:
	void SetCurrentSprite(SSprite sprite);
	void SetDefaultSprite(SSprite sprite);
	[[nodiscard]] SSprite GetDefaultSprite() const;
protected:
	SSprite _currentSprite;
	SSprite _defaultSprite;
};