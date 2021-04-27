#pragma once
#include <string>
#include <vector>

typedef std::vector<std::string> TokenizedStr;

class StringConverter
{
public:
    static std::wstring StrToWStr(const std::string& str) noexcept;
    static std::string WStrToStr(const std::wstring& wstr) noexcept;
    static std::wstring WcsToMbs(const char* str) noexcept;
    static std::string MbsToWcs(const wchar_t* wstr) noexcept;
    static std::vector<std::string> Tokenize(const std::string* str, char tokenChar = ' ');
    static unsigned long long HashStr(const std::string& str) noexcept;
};

