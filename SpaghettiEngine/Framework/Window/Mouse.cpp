#include "Mouse.h"

PMouse Mouse::instance = nullptr;

#pragma region public
bool Mouse::IsLeftPress() const noexcept
{
	return m_bLeftButtonPress;
}

bool Mouse::IsRightPress() const noexcept
{
	return m_bRightButtonPress;
}

bool Mouse::IsMiddlePress() const noexcept
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
	default:
		return false;
	}
}

bool Mouse::IsEmpty() const noexcept
{
	return m_qBuffer.empty();
}

MouseEvent Mouse::Read() noexcept
{
	if (!m_qBuffer.empty())
	{
		MouseEvent e = m_qBuffer.front();
		m_qBuffer.pop();
		return e;
	}
	else
	{
		return MouseEvent();
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
	AddEvent(MouseEvent::Type::Move, ptPos );
}

void Mouse::OnLeave() noexcept
{
	m_bIsInside = false;
	AddEvent(MouseEvent::Type::Leave );
}

void Mouse::OnEnter() noexcept
{
	m_bIsInside = true;
	AddEvent(MouseEvent::Type::Enter );
}

void Mouse::OnLeftPress( const Point& ptPos ) noexcept
{
	m_bLeftButtonPress = true;
	AddEvent(MouseEvent::Type::LPress, ptPos );
}

void Mouse::OnLeftRelease( const Point& ptPos ) noexcept
{
	m_bLeftButtonPress = false;
	AddEvent(MouseEvent::Type::LRelease, ptPos );
}

void Mouse::OnRightPress( const Point& ptPos ) noexcept
{
	m_bRightButtonPress = true;
	AddEvent(MouseEvent::Type::RPress );
}

void Mouse::OnRightRelease( const Point& ptPos ) noexcept
{
	m_bRightButtonPress = false;
	AddEvent(MouseEvent::Type::RRelease, ptPos );
}

void Mouse::OnMiddlePress( const Point& ptPos ) noexcept
{
	m_bMiddleButtonPress = true;
	AddEvent(MouseEvent::Type::MPress, ptPos );
}

void Mouse::OnMiddleRelease( const Point& ptPos ) noexcept
{
	m_bMiddleButtonPress = false;
	AddEvent(MouseEvent::Type::MRelease, ptPos );
}

void Mouse::OnWheelUp() noexcept
{
	AddEvent(MouseEvent::Type::WheelUp );
}

void Mouse::OnWheelDown() noexcept
{
	AddEvent(MouseEvent::Type::WheelDown );
}

void Mouse::AddEvent( const MouseEvent::Type& tEventType ) noexcept
{
	m_qBuffer.push(MouseEvent( tEventType, *this ) );
	TrimBuffer();
}

void Mouse::AddEvent( const MouseEvent::Type& tEventType, const Point& ptPos ) noexcept
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
PMouse Mouse::GetInstance() noexcept
{
	if (!instance)
		instance = new Mouse();
	return instance;
}
#pragma endregion

MouseEvent::MouseEvent() noexcept
	:
	m_tType(Type::Invalid),
	m_bIsRightPress(false),
	m_bIsLeftPress(false),
	m_bIsMiddlePress(false),
	m_ptPosition(Point(0, 0))
{}

MouseEvent::MouseEvent(Type tType, const Mouse& mParent) noexcept
	: m_tType(tType),
	m_bIsRightPress(mParent.IsRightPress()),
	m_bIsLeftPress(mParent.IsLeftPress()),
	m_bIsMiddlePress(mParent.IsMiddlePress()),
	m_ptPosition(mParent.GetPosition())
{}

bool MouseEvent::IsValid() const noexcept
{
	return m_tType != Type::Invalid;
}

MouseEvent::Type MouseEvent::GetType() const noexcept
{
	return m_tType;
}

Point MouseEvent::GetPosition() const noexcept
{
	return m_ptPosition;
}

int MouseEvent::GetPosX() const noexcept
{
	return m_ptPosition.x;
}

int MouseEvent::GetPosY() const noexcept
{
	return m_ptPosition.y;
}

bool MouseEvent::IsLeftPressed() const noexcept
{
	return m_bIsLeftPress;
}

bool MouseEvent::IsRightPressed() const noexcept
{
	return m_bIsRightPress;
}

bool MouseEvent::IsMiddlePressed() const noexcept
{
	return m_bIsMiddlePress;
}
