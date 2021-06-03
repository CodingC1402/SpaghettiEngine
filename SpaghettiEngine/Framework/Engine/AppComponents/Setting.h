#pragma once
#include "Plane2D.h"
#include "CornException.h"
#include "SMath.h"
#include <string>

class Setting
{
public:
	class AppSettingException : public CornDescriptionException
	{
	public:
		AppSettingException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static Setting* GetInstance();
	static Plane2D::Size& GetResolution();
	static Plane2D::Size& GetHalfResolution();
	static const wchar_t* GetAppName();
	static bool IsResolutionPixelPerfect();
	static bool IsWorldPointPixelPerfect();
	static float GetFps();
	
	static constexpr bool IsDebugMode();
protected:
	~Setting() = default;
	Setting();
	void Load();
protected:
	Plane2D::Size resolution;
	Plane2D::Size halfResolution;
	std::wstring name;
	bool isResolutionPixelPerfect = false;
	bool isWorldPointPixelPerfect = false;
	float fps;

	static Setting* __instance;
	
#ifdef _DEBUG
	static constexpr bool _isDebugMode = true;
#else
	static constexpr bool _isDebugMode = false;
#endif
};

constexpr bool Setting::IsDebugMode()
{
	return true;
}

#define APPSETTING_EXCEPT(description) Setting::AppSettingException(__LINE__,__FILE__,description)
