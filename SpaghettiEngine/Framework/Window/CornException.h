#pragma once
#include <exception>
#include <string>
#include <sstream>
#include "CornWnd.h"
#include "StringConverter.h"

class CornException : public std::exception
{
protected:
	const char *what() const noexcept override;
public:
	CornException( int line, const char* file ) noexcept;
	virtual const wchar_t *What() const noexcept;
	virtual const wchar_t *GetType() const noexcept;
	int GetLine() const noexcept;
	const std::wstring &GetFile() const noexcept;
	std::wstring GetOriginString() const noexcept;
private:
	int m_Line;
	std::wstring m_File;
protected:
	mutable std::wstring whatBuffer;
};

class CornDescriptionException : CornException
{
public:
	CornDescriptionException(int line, const char* file, std::wstring description) noexcept;
	virtual const wchar_t* GetType() const noexcept override;
	virtual const wchar_t* What() const noexcept override;
	std::wstring GetErrorString() const noexcept;
private:
	std::wstring description;
};

#define CORN_EXCEPT_WITH_DESCRIPTION(description) CornDescriptionException(__LINE__,__FILE__,description)