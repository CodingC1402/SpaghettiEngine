#pragma once

#include "CornWnd.h"
#include "Plane2D.h"
#include <queue>
#include <memory>

class Mouse;
typedef Mouse* PMouse;
typedef std::shared_ptr<Mouse> SMouse;

using Plane2D::Point;

class Mouse
{
	friend class Window;
public:
	class Event
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
		Event() noexcept
			: 
			m_tType( Type::Invalid ),
			m_bIsRightPress( false ),
			m_bIsLeftPress( false ),
			m_bIsMiddlePress( false ),
			m_ptPosition( Point(0, 0) )
		{}

		Event( Type tType, const Mouse& mParent ) noexcept
			: m_tType( tType ),
			m_bIsRightPress( mParent.m_bRightButtonPress ),
			m_bIsLeftPress( mParent.m_bLeftButtonPress ),
			m_bIsMiddlePress( mParent.m_bMiddleButtonPress ),
			m_ptPosition( mParent.m_ptPosition )
		{}

		bool IsValid() const noexcept
		{
			return m_tType != Type::Invalid;
		}
		Type GetType() const noexcept
		{
			return m_tType;
		}
		Point GetPosition() const noexcept
		{
			return m_ptPosition;
		}
		int GetPosX() const noexcept
		{
			return m_ptPosition.x;
		}
		int GetPosY() const noexcept
		{
			return m_ptPosition.y;
		}
		bool IsLeftPressed() const noexcept
		{
			return m_bIsLeftPress;
		}
		bool IsRightPressed() const noexcept
		{
			return m_bIsRightPress;
		}
		bool IsMiddlePressed() const noexcept
		{
			return m_bIsMiddlePress;
		}
	};
public:
	Mouse( const Mouse& ) = delete;
	Mouse& operator= ( const Mouse& ) = delete;

	// Just for more utility don't use these in game 
	// because these two prevent key customization
	bool	IsLeftPress		();
	bool	IsRightPress	();
	bool	IsMiddlePress	();
	// end

	bool	IsInside		();
	bool	IsButtonPress	( unsigned char ucCode );
	bool	IsEmpty			() const noexcept;
	Event	Read			() noexcept;
	void	Clear			() noexcept;
	Point	GetPosition		() const noexcept;
	int		GetPosX			() const noexcept;
	int		GetPosY			() const noexcept;
private:
	Mouse() = default;
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

	void AddEvent			( const Event::Type& tEventType ) noexcept;
	void AddEvent			( const Event::Type& tEventType, const Point& ptPos ) noexcept;
	void TrimBuffer			() noexcept;

	static PMouse Create();
private:
	static constexpr unsigned int m_uiBufferSize = 16u;
	Point m_ptPosition;
	bool m_bMiddleButtonPress	= false;
	bool m_bLeftButtonPress		= false;
	bool m_bRightButtonPress	= false;
	bool m_bIsInside			= false;
	std::queue<Event> m_qBuffer;
};