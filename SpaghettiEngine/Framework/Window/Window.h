#pragma once
#include "CornWnd.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "Debug.h"
#include "Timer.h"
#include "CornException.h"
#include "StringConverter.h"
#include "..\..\resource.h"

#include <memory>

typedef class Window
{
public:
	class Exception : public CornException
	{
	public:
		Exception( int line, const wchar_t *file, HRESULT hr ) noexcept;
		virtual const wchar_t *GetType() const noexcept override;
		virtual const wchar_t *What() const noexcept override;
		static std::wstring TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
	private:
		HRESULT m_HResult;
	};
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
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;

	bool SetName( const wchar_t* wcWndName ) const noexcept;
	const wchar_t* GetName() const noexcept;

	KeyBoard& GetKeyBoard() const noexcept;
	Mouse& GetMouse() const noexcept;

	void Destroy();
	static Window* Create(int iWidth, int iHeight, const wchar_t* iName);
	static DWORD ProcessMessages();
private:
	Window(int iWidth, int iHeight, const wchar_t* iName) noexcept;

	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT CALLBACK HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
private:
	std::wstring originalName;
	int m_iWidth;
	int m_iHeight;
	HWND m_hWnd;
	mutable KeyBoard m_kbKeyInput;
	mutable Mouse m_mMouseInput;
} *PWindow;

typedef std::unique_ptr<Window> UWindow;
typedef std::shared_ptr<Window> SWindow;

#define CWND_EXCEPT( hr ) Window::Exception(__LINE__,__FILE__,hr)
#define CWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError())