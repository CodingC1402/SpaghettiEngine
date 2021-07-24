#include "SingleUseSegmentScript.h"

REGISTER_FINISH(SingleUseSegmentScript, SegmentScript) {}

void SingleUseSegmentScript::OnStart()
{
	Spawn();
}

void SingleUseSegmentScript::OnEnd()
{
	DeSpawn();
}
