#pragma once 

struct tagRECT;

class Rect
{
public:
	[[nodiscard]] tagRECT GetDirectXRect() const noexcept;
public:
	long top;
	long right;
	long bottom;
	long left;
};