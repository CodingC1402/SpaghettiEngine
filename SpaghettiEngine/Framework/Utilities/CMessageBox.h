#pragma once
#include <string>

// fork you wintard >:( wasting my forking morning
class CMessageBox
{
public:
	class Flags
	{
	public:
		// Buttons
		static constexpr auto ABORTRETRYIGNORE	= 0x00000002L;
		static constexpr auto CANCELTRYCONTINUE	= 0x00000006L;
		static constexpr auto HELP				= 0x00004000L;
		static constexpr auto OK				= 0x00000000L;
		static constexpr auto OKCANCEL			= 0x00000001L;
		static constexpr auto RETRYCANCEL		= 0x00000005L;
		static constexpr auto YESNO				= 0x00000004L;
		static constexpr auto YESNOCANCEL		= 0x00000003L;

		// Icons
		static constexpr auto ICONEXCLAMATION	= 0x00000030L;
		static constexpr auto ICONWARNING		= 0x00000030L;
		static constexpr auto ICONINFORMATION	= 0x00000040L;
		static constexpr auto ICONASTERISK		= 0x00000040L;
		static constexpr auto ICONQUESTION		= 0x00000020L;
	};
public:
	CMessageBox(const std::wstring& text, const std::wstring& type, const long& flag);
	CMessageBox(const std::string& text, const std::string& type, const long& flag);

	[[nodiscard]] std::wstring GetTextWStr() const noexcept;
	[[nodiscard]] std::wstring GetTypeWStr() const noexcept;
	[[nodiscard]] std::string GetTextStr() const noexcept;
	[[nodiscard]] std::string GetTypeStr() const noexcept;
	[[nodiscard]] long GetFlag() const noexcept;

	void SetText(const std::wstring& text) noexcept;
	void SetText(const std::string& text) noexcept;
	void SetType(const std::wstring& type) noexcept;
	void SetType(const std::string& type) noexcept;
	void SetFlag(const long& flag) noexcept;

	void Show() const;
private:
	std::wstring _text;
	std::wstring _type;
	long _flag;
};