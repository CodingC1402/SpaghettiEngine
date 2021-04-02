#pragma once
#include "CornException.h"
#include "Window.h"
#include "StringConverter.h"
#include "Sprite.h"
#include <vector>
#include <d3d9.h>
#include <memory>

/// <summary>
/// Singleton directx9 wrapper
/// </summary>

typedef class Graphics* PGraphics;
typedef LPDIRECT3D9 DX;
typedef LPDIRECT3DDEVICE9 DXDev;
typedef D3DPRESENT_PARAMETERS DXPresentPara;
typedef D3DDISPLAYMODE DisplayMode;


class Graphics
{
	friend class App;
public:
	class GraphicException : public CornException
	{
	public:
		GraphicException(int line, const char* file, const wchar_t* discription) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
		std::wstring GetErrorString() const noexcept;
	private:
		const wchar_t* discription;
	};
public:
	static PGraphics GetInstance();
	static void ToFullScreenMode();
	static void ToWindowMode();
protected:
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void CreateResource();
	void ReleaseResource();

	SWindow GetCurrentWindow() const noexcept;

	void Init(STimer timer, int fps);
	void Render();

	HRESULT Begin() noexcept;
	bool End();
	bool Reset();

	void GetDisplayModesOfCurrentScreen();

	Graphics() noexcept;
	~Graphics() noexcept;
protected:
	SWindow wnd;
	RECT restoreRec;
	bool isFullScreen = false;

	DX dx = NULL;
	DXDev dxdev = NULL;
	DXPresentPara dxpp;
	std::vector<DisplayMode> mode;

	bool isDeviceLost = false;

	STimer timer;
	double delayPerFrame;
	double timeSinceLastFrame;
	std::queue<Sprite> buffer;

	// Temp
	int index = 2;
	int delta = -1;
	int rgb[3] = { 255, 0, 1 };
	bool jump = true;
	// EndTemp

	static PGraphics __instance;
};

#define GRAPHICS_EXCEPT(discription) Graphics::GraphicException(__LINE__,__FILE__,discription)