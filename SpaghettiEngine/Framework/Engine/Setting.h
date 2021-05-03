#pragma once
#include "Plane2D.h"
#include "CornException.h"
#include "ExMath.h"
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
	static Plane2D::Size GetResolution();
	static const wchar_t* GetAppName();
	static bool IsResolutionPixelPerfect();
	static bool IsWorldPointPixelPerfect();
	static float GetFps();
	
	static constexpr bool IsDebugMode();
	static constexpr ULL CreateTopLevelID(const unsigned& localID, const unsigned& componentType, const unsigned& prefabIndex = 0);
protected:
	~Setting() = default;
	Setting();
	void Load();
public:
	static constexpr auto _bitForLocalId = 40u;
	static constexpr auto _bitForPrefabId = 16u;
	static constexpr auto _bitForComponentType = 8u;

#ifdef _DEBUG
	static constexpr auto _max = 0xFFFFFFFFFFFFFFFFU;
	static constexpr auto _errorMaskLocalId = _max << _bitForLocalId;
	static constexpr auto _errorMaskPrefabIndex = _max << _bitForPrefabId;
	static constexpr auto _errorMaskComponentType = _max << _bitForComponentType;
#endif
protected:
	Plane2D::Size resolution;
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
	return _isDebugMode;
}

constexpr ULL Setting::CreateTopLevelID(const unsigned& localID, const unsigned& componentType,
	const unsigned& prefabIndex)
{
	return localID | (static_cast<ULL>(prefabIndex) << _bitForLocalId) | (static_cast<ULL>(prefabIndex) << (_bitForLocalId + _bitForPrefabId));
}

#define APPSETTING_EXCEPT(description) Setting::AppSettingException(__LINE__,__FILE__,description)
