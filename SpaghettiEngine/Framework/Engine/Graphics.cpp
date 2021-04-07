#include "Graphics.h"
#include "Monitor.h"
#include "Setting.h"
#include "json.hpp"
#include <fstream>
#include <d3dx9.h>
#include <d3dx9math.h>

PGraphics Graphics::__instance = nullptr;

Graphics::GraphicException::GraphicException(int line, const char* file, std::wstring discription) noexcept
	: 
	CornDiscriptionException(line, file, discription)
{}

const wchar_t* Graphics::GraphicException::GetType() const noexcept
{
	return L"∑(O_O;) Graphic Exception";
}

PGraphics Graphics::GetInstance()
{
	if (!__instance)
		__instance = new Graphics();
	return __instance;
}

void Graphics::ToFullScreenMode()
{
	__instance->FullScreen();
}

void Graphics::ToWindowMode()
{
	__instance->Window();
}

void Graphics::DrawSprite(const SSprite& renderSprite, const Plane2D::Rect& desRect)
{
	__instance->buffer.push(renderSprite);
	__instance->desRects.push(desRect);
}

void Graphics::LoadTexture(PDx9Texture& rTexture, const std::string& path, const D3DCOLOR &keyColor)
{
	HRESULT result;
	std::wstring wPath = StringConverter::StrToWStr(path);
	D3DXIMAGE_INFO info;

	result = D3DXGetImageInfoFromFile(wPath.c_str(), &info); 

	if (result != D3D_OK)
		throw GRAPHICS_EXCEPT_CODE(result);

	PGraphics gfx = __instance;
	result = D3DXCreateTextureFromFileEx(
		gfx->dxdev,
		wPath.c_str(),
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		keyColor,
		&info,
		NULL,
		&rTexture
		);

	if (result != D3D_OK)
		throw GRAPHICS_EXCEPT_CODE(result);
}

void Graphics::CreateResource()
{
	dx->CreateDevice(
		videoAdapter,
		D3DDEVTYPE_HAL,
		wnd->GetContentWndHandler(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&dxpp,
		&dxdev
	);

	if (!dx)
		throw GRAPHICS_EXCEPT(L"Can't initialize directX properly");
	if (!dxdev)
		throw GRAPHICS_EXCEPT(L"Can't initialize driectXDev, and there is no error code for this so... good luck fixing this ヽ(￣ω￣(。。 )ゝ ");
}

void Graphics::ReleaseResource()
{
	if (dxdev)
		dxdev->Release();
	dxdev = nullptr;
}

bool Graphics::FullScreen()
{
	if (isFullScreen)
		return false;

	bool result = false;
	isFullScreen = true;
	wnd->ChangeWindowMode(true);
	return result;
}

void Graphics::Window()
{
	if (!isFullScreen)
		return;

	isFullScreen = false;
	wnd->ChangeWindowMode(false);
}

SGameWnd Graphics::GetCurrentWindow() const noexcept
{
	return wnd;
}

void Graphics::Init(STimer timer, int fps, ColorFormat colorFormat)
{
	if (fps <= 0)
		delayPerFrame = 0;
	else
		delayPerFrame = 1 / static_cast<double>(fps);

	dx = Direct3DCreate9(D3D_SDK_VERSION);
	this->timer = timer;
	this->resolution = Setting::GetResolution();

	ZeroMemory(&dxpp, sizeof(dxpp));

	wnd = SGameWnd(GameWnd::Create(L"SpaghettiEngine"));

	dxpp.Windowed = TRUE; // thể hiện ở chế độ cửa sổ
	dxpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dxpp.BackBufferFormat = static_cast<D3DFORMAT>(colorFormat);
	dxpp.BackBufferCount = 1;
	dxpp.BackBufferWidth = resolution.width;
	dxpp.BackBufferHeight = resolution.height;
	dxpp.hDeviceWindow = wnd->GetContentWndHandler();

	CreateResource();
}


void Graphics::Render()
{
	timeSinceLastFrame += timer->GetDeltaTime();
	if (timeSinceLastFrame < delayPerFrame)
	{
		while (!buffer.empty())
			buffer.pop();
		return;
	}
	else
	{
		timeSinceLastFrame -= delayPerFrame * static_cast<int>(timeSinceLastFrame / delayPerFrame);

		rgb[index] += delta;
		if (!(rgb[index] & 0xFF))
		{
			if (jump)
			{
				index += 2;
				index %= 3;
				rgb[index] += 1;
				delta = 1;
				jump = false;
			}
			else
			{
				rgb[index] -= 1;
				index -= 1;
				if (index < 0)
					index = 2;
				delta = -1;
				jump = true;
			}
		}
	}

	dxdev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(rgb[0], rgb[1], rgb[2]), 1.0f, 0);

	if (Begin() != 0)
	{
		/// <summary>
		/// Render here
		/// </summary>
		
		LPDIRECT3DSURFACE9 backBuffer;
		dxdev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
		SSprite sprite;
		RECT desRect;
		Plane2D::Rect rect;
		while (!buffer.empty())
		{
			sprite = buffer.front();
			rect = desRects.front();
			desRect.left = rect.x;
			desRect.top = rect.y;
			desRect.right = desRect.left + rect.w;
			desRect.bottom = desRect.top + rect.h;
			desRects.pop();
			buffer.pop();
		}


		if (!End())
		{
			Reset();
		}
	}

	dxdev->Present(NULL, NULL, NULL, NULL);
}

HRESULT Graphics::Begin() noexcept
{
	return dxdev->BeginScene();
}

bool Graphics::End()
{
	// device alive
	if (!isDeviceLost)
	{
		dxdev->EndScene();
	}
	HRESULT hr = dxdev->TestCooperativeLevel();
	if (hr != D3D_OK)
	{
		if (hr == D3DERR_DEVICELOST)
		{
			isDeviceLost = true;
		}
		else if (hr == D3DERR_DRIVERINTERNALERROR)
		{
			PostQuitMessage(0);
			throw GRAPHICS_EXCEPT(L"directX driver internal error");
		}
		return false;
	}
	return true;
}

bool Graphics::Reset()
{
	HRESULT hr = dxdev->TestCooperativeLevel();
	if (hr == D3DERR_DEVICENOTRESET)
	{
		if (SUCCEEDED(dxdev->Reset(&dxpp)))
		{
			// reset success
			isDeviceLost = false;
			return true;
		}
	}
	// failed to reset
	return false;
}

void Graphics::UpdateCurrentVideoAdapter()
{
	HMONITOR monitor = Monitor::GetCurrentMonitor(wnd->GetHwnd());
	D3DFORMAT format = static_cast<D3DFORMAT>(colorFormat);
	adapterMode.clear();
	UINT adapterCount = dx->GetAdapterCount();
	for (UINT i = 0; i < adapterCount; i++)
	{
		if (dx->GetAdapterMonitor(i) == monitor)
		{
			videoAdapter = i;
			break;
		}
	}

	UINT modeCount = dx->GetAdapterModeCount(videoAdapter, format);
	for (UINT i = 0; i < modeCount; i++)
	{
		DisplayMode mode;
		if (dx->EnumAdapterModes(videoAdapter, format, i, &mode) == D3D_OK)
		{
			adapterMode.push_back(mode);
		}
	}
}

Graphics::Graphics() noexcept
{
	ZeroMemory(&dxpp, sizeof(dxpp));
}

Graphics::~Graphics() noexcept
{
	if (dx)
		dx->Release();
	dx = nullptr;
	ReleaseResource();
}

Graphics::GraphicCodeException::GraphicCodeException(int line, const char* file, HRESULT code) noexcept
	:
	CornException(line, file),
	code(code)
{}

const wchar_t* Graphics::GraphicCodeException::GetType() const noexcept
{
	return L"∑(O_O;) Graphic Exception";
}

const wchar_t* Graphics::GraphicCodeException::What() const noexcept
{
	std::wostringstream os;
	os << GetType() << std::endl;
	os << "[Discription] " << Translate() << std::endl;
	os << GetOriginString();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

const wchar_t* Graphics::GraphicCodeException::Translate() const noexcept
{
	switch (code)
	{
	case D3DERR_NOTAVAILABLE:
		return L"File is not available";
	case D3DERR_OUTOFVIDEOMEMORY:
		return L"Out of video memory(gpu)";
	case D3DERR_INVALIDCALL:
		return L"Invalid call";
	case D3DXERR_INVALIDDATA:
		return L"Invalid data";
	case E_OUTOFMEMORY:
		return L"Out of memory(ram)";
	default:
		return L": ^)";
	}
}

const HRESULT Graphics::GraphicCodeException::GetErrorCode() noexcept
{
	return code;
}
