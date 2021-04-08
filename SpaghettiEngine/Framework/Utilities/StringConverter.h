#pragma once
#include <string>

class StringConverter
{
public:
    static std::wstring StrToWStr(const std::string& str) noexcept;
    static std::string WStrToStr(const std::wstring& wstr) noexcept;
    static std::wstring WcsToMbs(const char* str) noexcept;
    static std::string MbsToWcs(const wchar_t* wstr) noexcept;
};

