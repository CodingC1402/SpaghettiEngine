#pragma once
#include "SegmentScript.h"

// Like segment script but won't re-spawn anything.
class SingleUseSegmentScript : public SegmentScript
{
public:
	void OnStart() override;
	void OnEnd() override;
private:
	REGISTER_START(SingleUseSegmentScript);
};

