#pragma once
#include "ScriptBase.h"
#include "LevelSegment.h"

//
// +-------------------------+
// |						 |
// |						 |			   +----------------------+
// |						 |			   |					  |
// |		SegmentA		 |			   |					  |
// |						 |			   |		SegmentB	  |
// |						 +--+-+--------+					  |
// |PreviousTrigger				|T|					   NextTrigger|
// +-------------------------+--+-+--------+----------------------+
// Use to switch on and off between two segment to make sure that only the needed segment
// is getting update.

class SegmentTrigger : public ScriptBase
{
public:
	enum class State
	{
		LoadingSegmentA,
		LoadingSegmentB,
	};
public:
	void OnCollide(CollideEvent& e) override;
	void OnFixedUpdate() override;

	[[nodiscard]] State GetState() const noexcept;
protected:
	void SetState() noexcept;
protected:
	// to calculate when the player enter;
	bool _playerInside = false;
	bool _shouldTrigger = true;

	State _state;

	SegmentTrigger* _previousTrigger;
	SegmentTrigger* _nextTrigger;

	LevelSegment* _segmentA;
	LevelSegment* _segmentB;
private:
	REGISTER_START(SegmentTrigger);
};

