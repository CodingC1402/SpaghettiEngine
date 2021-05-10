#include "Extra.h"

DestructSetFlagThreadSafe::DestructSetFlagThreadSafe(std::atomic_bool& flag, bool value)
	:
	_flag(flag),
	_value(value)
{}

DestructSetFlagThreadSafe::~DestructSetFlagThreadSafe()
{
	_flag = _value;
}
