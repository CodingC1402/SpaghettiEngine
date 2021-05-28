#include "Monitor.h"

using Plane2D::Rectangle;

Plane2D::Rectangle Monitor::GetCurrentMonitorInfo(const HWND& window)
{
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(info);
	if (GetMonitorInfoA(monitor, &info))
	{
		return Plane2D::Rectangle(info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top);
	}
	else
		return Plane2D::Rectangle(-1, -1, -1, -1);
}

Plane2D::Rectangle Monitor::GetPrimaryMonitorInfo()
{
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	return Plane2D::Rectangle(0, 0, w, h);
}

HMONITOR Monitor::GetCurrentMonitor(const HWND& window)
{
	return MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
}
