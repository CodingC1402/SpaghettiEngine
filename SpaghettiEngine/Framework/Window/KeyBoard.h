#pragma once
#include <vector>
#include <bitset>
#include <queue>
#include <memory>

class KeyBoard;
typedef int KeyCode;
typedef std::shared_ptr<KeyBoard> SKeyBoard;
typedef KeyBoard* PKeyBoard;

class KeyBoardEvent
{
public:
	enum class Type
	{
		Press,
		Release,
		Invalid
	};
private:
	Type m_tType;
	unsigned char m_ucCode;
	bool _isFirstTime = false;
public:
	KeyBoardEvent() noexcept;
	KeyBoardEvent(Type tType, unsigned char ucCode, bool isFirstTime) noexcept;

	[[nodiscard]]bool			IsFirstTime() const noexcept;
	[[nodiscard]]bool			IsPress() const noexcept;
	// Only work for is press cause ya know? How can you release twice or more.
	[[nodiscard]]bool			IsRelease() const noexcept;
	[[nodiscard]]bool			IsValid() const noexcept;
	[[nodiscard]]unsigned char	GetCode() const noexcept;
};

//Singleton
class KeyBoard
{
	friend class Window;
public:
	KeyBoard( const KeyBoard& ) = delete;
	KeyBoard& operator=( const KeyBoard& ) = delete;

	bool IsKeyEmpty() noexcept;
	KeyBoardEvent ReadKey() noexcept;
	void ClearKey() noexcept;

	wchar_t ReadChar() noexcept;
	bool IsCharEmpty() noexcept;
	void ClearChar() noexcept;

	void Clear() noexcept;

	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool IsAutoRepeatEnabled() noexcept;

	static PKeyBoard GetInstance() noexcept;
	static KeyBoard Destroy() noexcept;
private:
	KeyBoard() = default;
	~KeyBoard() = default;

	void OnKeyPressed( unsigned char ucKeycode, int lParam ) noexcept;
	void OnKeyRelease( unsigned char ucKeycode, int lParam ) noexcept;
	void OnChar( wchar_t  c ) noexcept;
	void OnLostFocus() noexcept;

	template<typename T>
	static void TrimBuffer( std::queue<T>& buffer ) noexcept;
private:
	static constexpr unsigned int m_bufferSize = 16u;
	bool m_autoRepeatEnabled = false;
	std::queue<KeyBoardEvent> m_qKeyBuffer;
	std::queue<wchar_t> m_qCharBuffer;

	static PKeyBoard instance;
};