#pragma once
#include "CornException.h"
#include "GameWnd.h"
#include "Sprite.h"
#include <vector>
#include <d3d9.h>

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
	enum class ColorFormat
	{
		ARGB32Bit = 21,
		RGB32Bit = 22,
	};

	class GraphicException : public CornDiscriptionException
	{
	public:
		GraphicException(int line, const char* file, std::wstring discription) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static PGraphics GetInstance();
	static void ToFullScreenMode();
	static void ToWindowMode();
	static void Render(const SSprite& renderSprite, const Plane2D::Rect& desRect); // Render Sprite
protected:
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void CreateResource();
	void ReleaseResource();

	bool FullScreen();
	void Window();
	SGameWnd GetCurrentWindow() const noexcept;

	void Init(STimer timer, int fps, ColorFormat colorFormat);
	void Render();

	HRESULT Begin() noexcept;
	bool End();
	bool Reset();

	void UpdateCurrentVideoAdapter();

	Graphics() noexcept;
	~Graphics() noexcept;
protected:
	SGameWnd wnd;
	RECT restoreRec;
	bool isFullScreen = false;
	Size resolution;

	DX dx = NULL;
	DXDev dxdev = NULL;
	DXPresentPara dxpp;
	ColorFormat colorFormat = ColorFormat::RGB32Bit;
	UINT videoAdapter = D3DADAPTER_DEFAULT;
	std::vector<DisplayMode> adapterMode;

	bool isDeviceLost = false;

	STimer timer;
	double delayPerFrame;
	double timeSinceLastFrame;

	std::queue<SSprite> buffer;
	std::queue<RECT> desRects;

	// Temp
	int index = 2;
	int delta = -1;
	int rgb[3] = { 255, 0, 1 };
	bool jump = true;
	// EndTemp

	static PGraphics __instance;
};

#define GRAPHICS_EXCEPT(discription) Graphics::GraphicException(__LINE__,__FILE__,discription)