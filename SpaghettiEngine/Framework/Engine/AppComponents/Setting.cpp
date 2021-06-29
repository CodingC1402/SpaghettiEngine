#include "Setting.h"
#include "json.hpp"
#include "App.h"
#include "StringConverter.h"
#include "Physic.h"
#include "SpaghettiEnginePath.h"
#include <fstream>

Setting* Setting::__instance = nullptr;

Setting* Setting::GetInstance()
{
    if (!__instance)
        __instance = new Setting();
    return __instance;
}

Plane2D::Size& Setting::GetResolution()
{
	return GetInstance()->resolution;
}

Plane2D::Size& Setting::GetHalfResolution()
{
	return GetInstance()->halfResolution;
}

const wchar_t* Setting::GetAppName()
{
	return GetInstance()->name.c_str();
}

bool Setting::IsResolutionPixelPerfect()
{
	return GetInstance()->isResolutionPixelPerfect;
}

bool Setting::IsWorldPointPixelPerfect()
{
	return GetInstance()->isWorldPointPixelPerfect;
}

float Setting::GetFps()
{
	return GetInstance()->fps;
}

Setting::Setting()
{
    Load();
}

void Setting::Load()
{
	using namespace nlohmann;

	std::ifstream jsonFile(SystemPath::AppJsonPath);
	if (!jsonFile.is_open())
	{
		std::wostringstream os;
		os << L"File ";
		os << SystemPath::AppJsonPath;
		os << L" Doesn't exist";
		throw APPSETTING_EXCEPT(os.str());
	}

	try
	{
		constexpr const char* RESOLUTION = "Resolution";
		constexpr const char* FPS = "Fps";
		constexpr const char* NAME = "Name";
		constexpr const char* WIDTH = "Width";
		constexpr const char* HEIGHT = "Height";
		constexpr const char* PIXELPERFECT_RESOLUTION = "PixelPerfectResolution";
		constexpr const char* PIXELPERFECT_WORLDPOINT = "PixelPerfectWorldPoint";
		constexpr const char* CAPPEDLOOP = "CappedLoop";
		constexpr const char* GRAVITY = "Gravity";
		constexpr const char* PHYSICSTEP = "PhysicStep";
		
		json file;
		jsonFile >> file;

		Physic::SetGravity(file[GRAVITY].get<float>());
		Physic::SetStep(file[PHYSICSTEP].get<float>());
		resolution.width = file[RESOLUTION][WIDTH].get<int>();
		resolution.height = file[RESOLUTION][HEIGHT].get<int>();
		halfResolution.width = resolution.width / 2;
		halfResolution.height = resolution.height / 2;
		name = StringConverter::StrToWStr(file[NAME].get<std::string>());
		isResolutionPixelPerfect = file[PIXELPERFECT_RESOLUTION].get<bool>();
		isWorldPointPixelPerfect = file[PIXELPERFECT_WORLDPOINT].get<bool>();
		fps = file[FPS].get<float>();


		jsonFile.close();
	}
	catch (...)
	{
		std::wostringstream os;
		os << L"File ";
		os << SystemPath::AppJsonPath;
		os << L" doesn't have the right format";
		throw APPSETTING_EXCEPT(os.str());
	}
}


Setting::AppSettingException::AppSettingException(int line, const char* file, std::wstring description) noexcept
	:
	CornDescriptionException(line, file, description)
{}

const wchar_t* Setting::AppSettingException::GetType() const noexcept
{
	return L"(ʘдʘ╬) APP SETTING EXCEPTION, YOU FUCKED UP SOMETHING DIDN'T YOU?";
}
