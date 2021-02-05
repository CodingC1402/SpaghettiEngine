#include "Mouse.h"

#pragma region public
inline bool Mouse::IsLeftPress()
{
	return m_bLeftButtonPress;
}

inline bool Mouse::IsRightPress()
{
	return m_bRightButtonPress;
}

inline bool Mouse::IsMiddlePress()
{
	return m_bMiddleButtonPress;
}

inline bool Mouse::IsButtonPress( unsigned char ucCode )
{
	switch (ucCode)
	{
	case VK_LBUTTON:
		return m_bLeftButtonPress;
	case VK_RBUTTON:
		return m_bRightButtonPress;
	case VK_MBUTTON:
		return m_bMiddleButtonPress;
	}
}

inline bool Mouse::IsEmpty() const noexcept
{
	return m_qBuffer.empty();
}

inline Mouse::Event Mouse::Read() noexcept
{
	if (!m_qBuffer.empty())
	{
		Event e = m_qBuffer.front();
		m_qBuffer.pop();
		return e;
	}
	else
	{
		return Event::Event();
	}
}

void Mouse::Clear() noexcept
{
	while (!m_qBuffer.empty())
	{
		m_qBuffer.pop();
	}
}

inline Point Mouse::GetPosition() const noexcept
{
	return m_ptPosition;
}

inline int Mouse::GetPosX() const noexcept
{
	return m_ptPosition.x;
}

inline int Mouse::GetPosY() const noexcept
{
	return m_ptPosition.y;
}
#pragma endregion

#pragma region window
inline void Mouse::OnMove( const Point& ptPos ) noexcept
{
	AddEvent( Event::Type::Move, ptPos );
}

inline void Mouse::OnLeftPress( const Point& ptPos ) noexcept
{
	m_bLeftButtonPress = true;
	AddEvent( Event::Type::LPress, ptPos );
}

inline void Mouse::OnLeftRelease( const Point& ptPos ) noexcept
{
	m_bLeftButtonPress = false;
	AddEvent( Event::Type::LRelease, ptPos );
}

inline void Mouse::OnRightPress( const Point& ptPos ) noexcept
{
	m_bRightButtonPress = true;
	AddEvent( Event::Type::RPress );
}

inline void Mouse::OnRightRelease( const Point& ptPos ) noexcept
{
	m_bRightButtonPress = false;
	AddEvent( Event::Type::RRelease, ptPos );
}

inline void Mouse::OnMiddlePress( const Point& ptPos ) noexcept
{
	m_bMiddleButtonPress = true;
	AddEvent( Event::Type::MPress, ptPos );
}

inline void Mouse::OnMiddleRelease( const Point& ptPos ) noexcept
{
	m_bMiddleButtonPress = false;
	AddEvent( Event::Type::MRelease, ptPos );
}

inline void Mouse::OnWheelUp( const Point& ptPos ) noexcept
{
	AddEvent( Event::Type::WheelUp );
}

inline void Mouse::OnWheelDown( const Point& ptPos ) noexcept
{
	AddEvent( Event::Type::WheelDown );
}

inline void Mouse::AddEvent( const Event::Type& tEventType ) noexcept
{
	m_qBuffer.push( Event( Event::Type::LPress, *this ) );
	TrimBuffer();
}

inline void Mouse::AddEvent( const Event::Type& tEventType, const Point& ptPos ) noexcept
{
	m_ptPosition = ptPos;
	AddEvent( tEventType );
}

inline void Mouse::TrimBuffer() noexcept
{
	while (m_qBuffer.size() > m_uiBufferSize)
	{
		m_qBuffer.pop();
	}
}
#pragma endregion
