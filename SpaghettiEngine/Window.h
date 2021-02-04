#pragma once
#include "SpaghettiWin.h"
#include "WStrUtils.h"
#include "KeyBoard.h"
#include "Debug.h"
#include "Time.h"

class Window
{
private:
	// Singleton class
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		Window& operator=( const WindowClass& ) = delete;
		static constexpr const wchar_t* m_pwcWndClassName = L"SpaghettiEngine";
		static WindowClass m_wcWinClass;
		HINSTANCE m_hInst;
	};
public:
	Window( int iWidth, int iHeight, const wchar_t* iName ) noexcept;
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;

	bool SetText( const wchar_t* wcWndName );
private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT CALLBACK HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
private:
	int m_iWidth;
	int m_iHeight;
	HWND m_hWnd;

	KeyBoard m_kbInput;
};

