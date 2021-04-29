#pragma once
#include <vector>
#include <memory>

typedef class Transition* PTransition;
typedef std::shared_ptr<Transition> STransition;

typedef class Trigger* PTrigger;
typedef std::shared_ptr<Trigger> STrigger;

typedef class State* PState;
typedef std::shared_ptr<State> SState;

typedef class AnimationController* PAnimationController;
typedef std::shared_ptr<AnimationController> SAnimationController;

class Trigger
{
public:

protected:
	PAnimationController _owner;
};
class Transition
{
	std::vector<STrigger> _triggers;
};
class State
{
	std::vector<STransition> _transitions;
};
class AnimationController
{
protected:
	SState currentState;
	
};