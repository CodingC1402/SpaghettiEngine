#include "StringConverter.h"
#include <fstream>
#include <sstream>

std::wstring StringConverter::StrToWStr(const std::string& str) noexcept
{
    std::wostringstream os;
    os << str.c_str();
    return os.str();
}

std::string StringConverter::WStrToStr(const std::wstring& wstr) noexcept
{
    std::wostringstream os;
    std::string rStr = "";
    for (int i = 0; i < wstr.size(); i++)
    {
        rStr += os.narrow(wstr[i], 'ø');
    }
    return rStr;
}

std::wstring StringConverter::WcsToMbs(const char* str) noexcept
{
    return StrToWStr(std::string(str));
}

std::string StringConverter::MbsToWcs(const wchar_t* wstr) noexcept
{
    return WStrToStr(std::wstring(wstr));
}

std::vector<std::string> StringConverter::Tokenize(const std::string* str, char tokenChar)
{
    std::stringstream strStream(*str);
    std::string token;
    std::vector<std::string> rTokens;

    while (std::getline(strStream, token, tokenChar))
    {
        rTokens.push_back(token);
    }

    return rTokens;
}
