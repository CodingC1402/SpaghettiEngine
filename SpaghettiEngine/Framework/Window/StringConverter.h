#pragma once
#include <codecvt>
#include <string>

#define UTF_8 65001

class StringConverter
{
public:
    static std::wstring StrToWStr(const std::string& str) noexcept;
    static std::string WStrToStr(const std::wstring& wstr) noexcept;
    static std::wstring WcsToMbs(const char* str) noexcept;
    static std::string MbsToWcs(const wchar_t* wstr) noexcept;
protected:
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
};

