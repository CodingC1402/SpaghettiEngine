#pragma once
#include <atomic>

class DestructSetFlagThreadSafe
{
public:
	DestructSetFlagThreadSafe(std::atomic_bool &flag, bool value);
	~DestructSetFlagThreadSafe();
protected:
	std::atomic_bool &_flag;
	bool _value;
};