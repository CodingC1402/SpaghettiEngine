#include "CMessageBox.h"
#include "StringConverter.h"
#include "CornWnd.h"

CMessageBox::CMessageBox(const std::wstring& text, const std::wstring& type, const long& flag)
{
	_text = text;
	_type = type;
	_flag = flag;
}

CMessageBox::CMessageBox(const std::string& text, const std::string& type, const long& flag)
{
	SetText(text);
	SetType(text);
	_flag = flag;
}

std::wstring CMessageBox::GetTextWStr() const noexcept
{
	return _text;
}

std::wstring CMessageBox::GetTypeWStr() const noexcept
{
	return _type;
}

std::string CMessageBox::GetTextStr() const noexcept
{
	return StringConverter::WStrToStr(_text);
}

std::string CMessageBox::GetTypeStr() const noexcept
{
	return StringConverter::WStrToStr(_type);
}

long CMessageBox::GetFlag() const noexcept
{
	return _flag;
}

void CMessageBox::SetText(const std::wstring& text) noexcept
{
	_text = text;
}

void CMessageBox::SetText(const std::string& text) noexcept
{
	_text = StringConverter::StrToWStr(text);
}

void CMessageBox::SetType(const std::wstring& type) noexcept
{
	_type = type;
}

void CMessageBox::SetType(const std::string& type) noexcept
{
	_type = StringConverter::StrToWStr(type);
}

void CMessageBox::SetFlag(const long& flag) noexcept
{
	_flag = flag;
}

void CMessageBox::Show() const
{
	// Yes I have to do this ~~
	MessageBox(nullptr, _text.c_str(), _type.c_str(), _flag);
}
