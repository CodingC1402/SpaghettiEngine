#include "CornException.h"

const char *CornException::what() const noexcept
{
    return nullptr;
}

CornException::CornException( int line, const wchar_t *file ) noexcept
    :
    m_Line( line),
    m_File ( file )
{}

const wchar_t *CornException::What() const noexcept
{
    std::wostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
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
