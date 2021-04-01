#include "KeyBoard.h"

PKeyBoard KeyBoard::instance = nullptr;

KeyBoard::Event KeyBoard::ReadKey() noexcept
{
	if ( m_qKeyBuffer.size() > 0u )
	{
		KeyBoard::Event e = m_qKeyBuffer.front();
		m_qKeyBuffer.pop();
		return e;
	}
	else
	{
		return KeyBoard::Event();
	}
}

bool KeyBoard::IsKeyEmpty() noexcept
{
	return m_qKeyBuffer.empty();
}

wchar_t KeyBoard::ReadChar() noexcept
{
	if ( m_qCharBuffer.size() > 0u )
	{
		unsigned char ucCode = m_qCharBuffer.front();
		m_qCharBuffer.pop();
		return ucCode;
	}
	else
	{
		return 0;
	}
}

bool KeyBoard::IsCharEmpty() noexcept
{
	return m_qCharBuffer.empty();
}

void KeyBoard::ClearKey() noexcept
{
	m_qKeyBuffer = std::queue<Event>();
}

void KeyBoard::ClearChar() noexcept
{
	m_qCharBuffer = std::queue<wchar_t>();
}

void KeyBoard::Clear() noexcept
{
	ClearKey();
	ClearChar();
}

void KeyBoard::EnableAutoRepeat() noexcept
{
	m_autoRepeatEnabled = true;
}

void KeyBoard::DisableAutoRepeat() noexcept
{
	m_autoRepeatEnabled = false;
}

bool KeyBoard::IsAutoRepeatEnabled() noexcept
{
	return m_autoRepeatEnabled;
}



void KeyBoard::OnKeyPressed( unsigned char ucKeycode ) noexcept
{
	m_qKeyBuffer.push( KeyBoard::Event( KeyBoard::Event::Type::Press, ucKeycode) );
	TrimBuffer( m_qKeyBuffer );
}

void KeyBoard::OnKeyRelease( unsigned char ucKeycode ) noexcept
{
	m_qKeyBuffer.push( KeyBoard::Event( KeyBoard::Event::Type::Release, ucKeycode ) );
	TrimBuffer( m_qKeyBuffer );
}

void KeyBoard::OnChar( wchar_t c ) noexcept
{
	m_qCharBuffer.push( c );
	TrimBuffer( m_qCharBuffer );
}

void KeyBoard::OnLostFocus() noexcept
{
	m_qKeyBuffer.push(Event());
	TrimBuffer(m_qKeyBuffer);
}

PKeyBoard KeyBoard::GetInstance() noexcept
{
	if (instance == nullptr)
		instance = new KeyBoard();
	return instance;
}

template<typename T>
void KeyBoard::TrimBuffer( std::queue<T>& buffer ) noexcept
{
	while ( buffer.size() > m_bufferSize )
	{
		buffer.pop();
	}
}