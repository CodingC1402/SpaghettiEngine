#pragma once
#include <codecvt>
#include <string>

#define UTF_8 65001

class StringConverter
{
public:
    static std::wstring StrToWStr(const std::string& str);
    static std::string WStrToStr(const std::wstring& wstr);
    static std::wstring WcsToMbs(const char* str);
    static std::string MbsToWcs(const wchar_t* wstr);
protected:
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
};

