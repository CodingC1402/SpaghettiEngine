#include "Setting.h"
#include "json.hpp"
#include "App.h"
#include "StringConverter.h"
#include "SpaghettiEnginePath.h"
#include <fstream>

Setting* Setting::__instance = nullptr;

Setting* Setting::GetInstance()
{
    if (!__instance)
        __instance = new Setting();
    return __instance;
}

Plane2D::Size Setting::GetResolution()
{
	return GetInstance()->resolution;
}

const wchar_t* Setting::GetAppName()
{
	return GetInstance()->name.c_str();
}

Setting::Setting()
{
    Load();
}

void Setting::Load()
{
	using namespace nlohmann;

	std::ifstream jsonFile(APPJSONPATH);
	if (!jsonFile.is_open())
	{
		std::wostringstream os;
		os << L"File ";
		os << APPJSONPATH;
		os << L" Doesn't exist";
		throw APPSETTING_EXCEPT(os.str());
	}

	try
	{
		json file;
		jsonFile >> file;

		resolution.width = file["Resolution"]["Width"].get<int>();
		resolution.height = file["Resolution"]["Height"].get<int>();
		name = StringConverter::StrToWStr(file["Name"].get<std::string>());
	}
	catch (...)
	{
		std::wostringstream os;
		os << L"File ";
		os << APPJSONPATH;
		os << L" doesn't have the right format";
		throw APPSETTING_EXCEPT(os.str());
	}
}


Setting::AppSettingException::AppSettingException(int line, const char* file, std::wstring discription) noexcept
	:
	CornDiscriptionException(line, file, discription)
{}

const wchar_t* Setting::AppSettingException::GetType() const noexcept
{
	return L"(ʘдʘ╬) APP SETTING EXCEPTION, YOU FUCKED UP SOMETHING DIDN'T YOU?";
}
