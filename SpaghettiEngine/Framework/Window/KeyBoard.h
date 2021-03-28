#pragma once
#include <vector>
#include <bitset>
#include <queue>

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
	public:
		Event() noexcept : m_tType( Type::Invalid ), m_ucCode( 0u ) {}
		Event( Type tType, unsigned char ucCode ) noexcept : m_tType( tType ), m_ucCode( ucCode ) {}

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
			return m_tType == Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return m_ucCode;
		}
	};
public:
	KeyBoard() = default;
	KeyBoard( const KeyBoard& ) = delete;
	KeyBoard& operator=( const KeyBoard& ) = delete;

	bool IsKeyPress( unsigned char ucKeycode ) noexcept;
	bool IsKeyEmpty() noexcept;
	Event ReadKey() noexcept;
	void ClearKey() noexcept;

	char ReadChar() noexcept;
	bool IsCharEmpty() noexcept;
	void ClearChar() noexcept;

	void Clear() noexcept;

	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool IsAutoRepeatEnabled() noexcept;
private:
	void OnKeyPressed( unsigned char ucKeycode ) noexcept;
	void OnKeyRelease( unsigned char ucKeycode ) noexcept;
	void OnChar( char  c ) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer( std::queue<T>& buffer ) noexcept;
private:
	static constexpr unsigned int m_uiKeys = 256u;
	static constexpr unsigned int m_bufferSize = 16u;
	bool m_autoRepeatEnabled = false;
	std::bitset<m_uiKeys> m_bsKeyStates;
	std::queue<Event> m_qKeyBuffer;
	std::queue<char> m_qCharBuffer;
};
