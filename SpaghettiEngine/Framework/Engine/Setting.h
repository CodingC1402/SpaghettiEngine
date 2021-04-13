#pragma once
#include "Plane2D.h"
#include "CornException.h"
#include <string>

class Setting
{
public:
	class AppSettingException : public CornDiscriptionException
	{
	public:
		AppSettingException(int line, const char* file, std::wstring discription) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static Setting* GetInstance();
	static Plane2D::Size GetResolution();
	static const wchar_t* GetAppName();
	static int GetCappedLoop();
	static bool IsResolutionPixelPerfect();
	static bool IsWorldPointPixelPerfect();
	static float GetFps();
protected:
	~Setting() = default;
	Setting();
	void Load();
protected:
	Plane2D::Size resolution;
	std::wstring name;
	bool isResolutionPixelPerfect = false;
	bool isWorldPointPixelPerfect = false;
	float fps;
	int cappedLoop;

	static Setting* __instance;
};

#define APPSETTING_EXCEPT(discription) Setting::AppSettingException(__LINE__,__FILE__,discription)