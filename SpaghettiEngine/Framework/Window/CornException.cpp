#include "CornException.h"

const char *CornException::what() const noexcept
{
    return nullptr;
}

CornException::CornException(int line, const char* file) noexcept
    :
    m_Line(line),
    m_File(StringConverter::StrToWStr(file))
{}

const wchar_t *CornException::What() const noexcept
{
    std::wostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString() << std::endl;
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const wchar_t *CornException::GetType() const noexcept
{
    return L"(っ´ω`)ﾉ(╥ω╥) Unknow CornException ";
}

int CornException::GetLine() const noexcept
{
    return m_Line;
}

const std::wstring &CornException::GetFile() const noexcept
{
    return m_File;
}

std::wstring CornException::GetOriginString() const noexcept
{
    std::wostringstream oss;
    oss << "[File] " << m_File << std::endl
        << "[Line] " << m_Line;
    return oss.str();
}

CornDescriptionException::CornDescriptionException(int line, const char* file, std::wstring description) noexcept : CornException(line, file)
{
    this->description = description;
}

const wchar_t* CornDescriptionException::GetType() const noexcept
{
    return L"＼(º □ º l|l)/ Exception with description";
}

const wchar_t* CornDescriptionException::What() const noexcept
{
    std::wostringstream oss;
    oss << GetOriginString() << std::endl
        << "[Description] " << GetErrorString() << std::endl;
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

std::wstring CornDescriptionException::GetErrorString() const noexcept
{
    return description;
}
