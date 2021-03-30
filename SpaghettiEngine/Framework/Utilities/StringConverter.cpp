#include "StringConverter.h"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> StringConverter::converter;

std::wstring StringConverter::StrToWStr(const std::string& str) noexcept
{
    std::wstring wstr = converter.from_bytes(str);
    return wstr;
}

std::string StringConverter::WStrToStr(const std::wstring& wstr) noexcept
{
    std::string str = converter.to_bytes(wstr);
    return str;
}

std::wstring StringConverter::WcsToMbs(const char* str) noexcept
{
    std::wstring wstr = converter.from_bytes(str);
    return wstr;
}

std::string StringConverter::MbsToWcs(const wchar_t* wstr) noexcept
{
    std::string str = converter.to_bytes(wstr);
    return str;
}