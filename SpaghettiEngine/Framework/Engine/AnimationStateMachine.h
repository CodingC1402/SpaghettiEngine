#pragma once
#include <vector>
#include <memory>

typedef class Transition* PTransition;
typedef std::shared_ptr<Transition> STransition;

typedef class Trigger* PTrigger;
typedef std::shared_ptr<Trigger> STrigger;

typedef class State* PState;
typedef std::shared_ptr<State> SState;

class Trigger
{

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
	SState currentState;
};