#include "Rect.h"
#include "CornDirectX.h"

tagRECT Rect::GetDirectXRect() const noexcept
{
	RECT rValue;
	rValue.top = top;
	rValue.right = right;
	rValue.bottom = bottom;
	rValue.left = left;
	return rValue;
}
