#include "Monitor.h"

Size Monitor::GetCurrentMonitorSize(const HWND& window)
{
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(info);
	if (GetMonitorInfoA(monitor, &info))
	{
		return Size(info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top);
	}
	else
		return Size(-1, -1);
}

Size Monitor::GetPrimaryMonitorSize()
{
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	return Size(w, h);
}
