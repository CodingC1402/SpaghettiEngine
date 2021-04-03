#pragma once
#include "Plane2D.h"

using Plane2D::Size;

class Monitor
{
public:
	static Plane2D::Rectangle GetCurrentMonitorInfo(const HWND& window);
	static Plane2D::Rectangle GetPrimaryMonitorInfo();
	static HMONITOR GetCurrentMonitor(const HWND& window);
};