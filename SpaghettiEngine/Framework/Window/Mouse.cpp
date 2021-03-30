#include "Mouse.h"

#pragma region public
bool Mouse::IsLeftPress()
{
	return m_bLeftButtonPress;
}

bool Mouse::IsRightPress()
{
	return m_bRightButtonPress;
}

bool Mouse::IsMiddlePress()
{
	return m_bMiddleButtonPress;
}

bool Mouse::IsInside()
{
	return m_bIsInside;
}

bool Mouse::IsButtonPress( unsigned char ucCode )
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

bool Mouse::IsEmpty() const noexcept
{
	return m_qBuffer.empty();
}

Mouse::Event Mouse::Read() noexcept
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

Point Mouse::GetPosition() const noexcept
{
	return m_ptPosition;
}

int Mouse::GetPosX() const noexcept
{
	return m_ptPosition.x;
}

int Mouse::GetPosY() const noexcept
{
	return m_ptPosition.y;
}
#pragma endregion

#pragma region window
void Mouse::OnMove( const Point& ptPos ) noexcept
{
	AddEvent( Event::Type::Move, ptPos );
}

void Mouse::OnLeave() noexcept
{
	m_bIsInside = false;
	AddEvent( Event::Type::Leave );
}

void Mouse::OnEnter() noexcept
{
	m_bIsInside = true;
	AddEvent( Event::Type::Enter );
}

void Mouse::OnLeftPress( const Point& ptPos ) noexcept
{
	m_bLeftButtonPress = true;
	AddEvent( Event::Type::LPress, ptPos );
}

void Mouse::OnLeftRelease( const Point& ptPos ) noexcept
{
	m_bLeftButtonPress = false;
	AddEvent( Event::Type::LRelease, ptPos );
}

void Mouse::OnRightPress( const Point& ptPos ) noexcept
{
	m_bRightButtonPress = true;
	AddEvent( Event::Type::RPress );
}

void Mouse::OnRightRelease( const Point& ptPos ) noexcept
{
	m_bRightButtonPress = false;
	AddEvent( Event::Type::RRelease, ptPos );
}

void Mouse::OnMiddlePress( const Point& ptPos ) noexcept
{
	m_bMiddleButtonPress = true;
	AddEvent( Event::Type::MPress, ptPos );
}

void Mouse::OnMiddleRelease( const Point& ptPos ) noexcept
{
	m_bMiddleButtonPress = false;
	AddEvent( Event::Type::MRelease, ptPos );
}

void Mouse::OnWheelUp() noexcept
{
	AddEvent( Event::Type::WheelUp );
}

void Mouse::OnWheelDown() noexcept
{
	AddEvent( Event::Type::WheelDown );
}

void Mouse::AddEvent( const Event::Type& tEventType ) noexcept
{
	m_qBuffer.push( Event( tEventType, *this ) );
	TrimBuffer();
}

void Mouse::AddEvent( const Event::Type& tEventType, const Point& ptPos ) noexcept
{
	m_ptPosition = ptPos;
	AddEvent( tEventType );
}

void Mouse::TrimBuffer() noexcept
{
	while (m_qBuffer.size() > m_uiBufferSize)
	{
		m_qBuffer.pop();
	}
}
PMouse Mouse::Create()
{
	return new Mouse();
}
#pragma endregion
