#include "KeyBoard.h"

PKeyBoard KeyBoard::instance = nullptr;

KeyBoardEvent KeyBoard::ReadKey() noexcept
{
	if ( m_qKeyBuffer.size() > 0u )
	{
		KeyBoardEvent e = m_qKeyBuffer.front();
		m_qKeyBuffer.pop();
		return e;
	}

	return KeyBoardEvent();
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
	m_qKeyBuffer = std::queue<KeyBoardEvent>();
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



void KeyBoard::OnKeyPressed( unsigned char ucKeycode, int lParam ) noexcept
{
	m_qKeyBuffer.push( KeyBoardEvent( KeyBoardEvent::Type::Press, ucKeycode, !(lParam & 0x40000000)));
	TrimBuffer( m_qKeyBuffer );
}

void KeyBoard::OnKeyRelease( unsigned char ucKeycode, int lParam ) noexcept
{
	m_qKeyBuffer.push( KeyBoardEvent( KeyBoardEvent::Type::Release, ucKeycode, !(lParam & 0x40000000)));
	TrimBuffer( m_qKeyBuffer );
}

void KeyBoard::OnChar( wchar_t c ) noexcept
{
	m_qCharBuffer.push( c );
	TrimBuffer( m_qCharBuffer );
}

void KeyBoard::OnLostFocus() noexcept
{
	m_qKeyBuffer.push(KeyBoardEvent());
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

KeyBoardEvent::KeyBoardEvent() noexcept
	: m_tType(Type::Invalid), m_ucCode(0u) 
{}

KeyBoardEvent::KeyBoardEvent(Type tType, unsigned char ucCode, bool isFirstTime) noexcept
	: m_tType(tType), m_ucCode(ucCode), _isFirstTime(isFirstTime) 
{}

bool KeyBoardEvent::IsFirstTime() const noexcept
{
	return _isFirstTime;
}

bool KeyBoardEvent::IsPress() const noexcept
{
	return m_tType == Type::Press;
}

bool KeyBoardEvent::IsRelease() const noexcept
{
	return m_tType == Type::Release;
}

bool KeyBoardEvent::IsValid() const noexcept
{
	return m_tType != Type::Invalid;
}

unsigned char KeyBoardEvent::GetCode() const noexcept
{
	return m_ucCode;
}