#pragma once
#include "Plane2D.h"

using Plane2D::Size;

class Monitor
{
public:
	static Size GetCurrentMonitorSize(const HWND& window);
	static Size GetPrimaryMonitorSize();
};