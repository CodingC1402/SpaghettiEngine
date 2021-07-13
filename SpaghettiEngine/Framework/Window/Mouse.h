#pragma once

#include "CornWnd.h"
#include "Plane2D.h"
#include <queue>
#include <memory>

class Mouse;
typedef Mouse* PMouse;
typedef std::shared_ptr<Mouse> SMouse;

using Plane2D::Point;

class MouseEvent
{
public:
	enum class Type
	{
		RPress,
		RRelease,
		LPress,
		LRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		Enter,
		Leave,
		Invalid
	};
private:
	Type m_tType;

	// save the state of other mouse button
	bool m_bIsRightPress;
	bool m_bIsLeftPress;
	bool m_bIsMiddlePress;

	Point m_ptPosition;
public:
	MouseEvent() noexcept;
	MouseEvent(Type tType, const Mouse& mParent) noexcept;

	[[nodiscard]]bool	IsValid() const noexcept;
	[[nodiscard]]Type	GetType() const noexcept;
	[[nodiscard]]Point	GetPosition() const noexcept;
	[[nodiscard]]int	GetPosX() const noexcept;
	[[nodiscard]]int	GetPosY() const noexcept;
	[[nodiscard]]bool	IsLeftPressed() const noexcept;
	[[nodiscard]]bool	IsRightPressed() const noexcept;
	[[nodiscard]]bool	IsMiddlePressed() const noexcept;
};

//Singleton
class Mouse
{
	friend class Window;
public:
	Mouse( const Mouse& ) = delete;
	Mouse& operator= ( const Mouse& ) = delete;

	// Just for more utility don't use these in game 
	// because these two prevent key customization
	bool	IsLeftPress		() const noexcept;
	bool	IsRightPress	() const noexcept;
	bool	IsMiddlePress	() const noexcept;
	// end

	bool		IsInside		();
	bool		IsButtonPress	( unsigned char ucCode );
	bool		IsEmpty			() const noexcept;
	MouseEvent	Read			() noexcept;
	void		Clear			() noexcept;
	Point		GetPosition		() const noexcept;
	int			GetPosX			() const noexcept;
	int			GetPosY			() const noexcept;

	static PMouse GetInstance() noexcept;
private:
	Mouse() = default;
	~Mouse() = default;

	void OnMove				( const Point& ptPos ) noexcept;
	void OnLeave			() noexcept;
	void OnEnter			() noexcept;
	void OnLeftPress		( const Point& ptPos ) noexcept;
	void OnLeftRelease		( const Point& ptPos ) noexcept;
	void OnRightPress		( const Point& ptPos ) noexcept;
	void OnRightRelease		( const Point& ptPos ) noexcept;
	void OnMiddlePress		( const Point& ptPos ) noexcept;
	void OnMiddleRelease	( const Point& ptPos ) noexcept;
	void OnWheelUp			() noexcept;
	void OnWheelDown		() noexcept;

	void AddEvent			( const MouseEvent::Type& tEventType ) noexcept;
	void AddEvent			( const MouseEvent::Type& tEventType, const Point& ptPos ) noexcept;
	void TrimBuffer			() noexcept;
private:
	static constexpr unsigned int m_uiBufferSize = 16u;
	Point m_ptPosition;
	bool m_bMiddleButtonPress	= false;
	bool m_bLeftButtonPress		= false;
	bool m_bRightButtonPress	= false;
	bool m_bIsInside			= false;
	std::queue<MouseEvent> m_qBuffer;

	static PMouse instance;
};