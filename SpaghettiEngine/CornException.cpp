#include "CornException.h"

CornException::CornException( int line, const char *file ) noexcept
    :
    m_Line( line),
    m_File ( file )
{}

const char *CornException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *CornException::GetType() const noexcept
{
    return "UwU! CornException";
}

int CornException::GetLine() const noexcept
{
    return m_Line;
}

const std::string &CornException::GetFile() const noexcept
{
    return m_File;
}

std::string CornException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << m_File << std::endl
        << "[Line] " << m_Line;
    return oss.str();
}
