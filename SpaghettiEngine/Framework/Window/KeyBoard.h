#pragma once
#include <vector>
#include <bitset>
#include <queue>
#include <memory>

class KeyBoard;
typedef int KeyCode;
typedef std::shared_ptr<KeyBoard> SKeyBoard;
typedef KeyBoard* PKeyBoard;


//Singleton
class KeyBoard
{
	friend class Window;
public:
	class Event
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
		Event() noexcept : m_tType( Type::Invalid ), m_ucCode( 0u ) {}
		Event( Type tType, unsigned char ucCode, bool isFirstTime ) noexcept : m_tType( tType ), m_ucCode( ucCode ), _isFirstTime(isFirstTime) {}

		bool IsFirstTime() const noexcept
		{
			return _isFirstTime;
		}
		bool IsPress() const noexcept
		{
			return m_tType == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return m_tType == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return m_tType != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return m_ucCode;
		}
	};
public:
	KeyBoard( const KeyBoard& ) = delete;
	KeyBoard& operator=( const KeyBoard& ) = delete;

	bool IsKeyEmpty() noexcept;
	Event ReadKey() noexcept;
	void ClearKey() noexcept;

	wchar_t ReadChar() noexcept;
	bool IsCharEmpty() noexcept;
	void ClearChar() noexcept;

	void Clear() noexcept;

	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool IsAutoRepeatEnabled() noexcept;

	static PKeyBoard GetInstance() noexcept;
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
	std::queue<Event> m_qKeyBuffer;
	std::queue<wchar_t> m_qCharBuffer;

	static PKeyBoard instance;
};