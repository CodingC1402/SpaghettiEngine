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

void Graphics::Draw(const PSpriteRenderer renderScript)
{
	__instance->renderBuffer.push_back(renderScript);
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
		gfx->renderDevice,
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

void Graphics::AddCamera(PCamera camera)
{
	__instance->cameraList.push_back(camera);
}

void Graphics::RemoveCamera(PCamera camera)
{
	size_t size = __instance->cameraList.size();
	auto iterator = __instance->cameraList.begin();
	while (size > 0)
	{
		if (*iterator == camera)
			__instance->cameraList.erase(iterator);
		std::advance(iterator, 1);
		size--;
	}
}

void Graphics::CreateResource()
{
	renderer->CreateDevice(
		videoAdapter,
		D3DDEVTYPE_HAL,
		wnd->GetContentWndHandler(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&presentParam,
		&renderDevice
	);

	HRESULT result = D3DXCreateSprite(renderDevice, &spriteHandler);
	if (FAILED(result))
		throw GRAPHICS_EXCEPT_CODE(result);

	if (!renderer)
		throw GRAPHICS_EXCEPT(L"Can't initialize directX properly");
	if (!renderDevice)
		throw GRAPHICS_EXCEPT(L"Can't initialize driectXDev, and there is no error code for this so... good luck fixing this ヽ(￣ω￣(。。 )ゝ ");
}

void Graphics::ReleaseResource()
{
	if (renderDevice)
		renderDevice->Release();
	if (spriteHandler)
		spriteHandler->Release();
	renderDevice = nullptr;
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

	renderer = Direct3DCreate9(D3D_SDK_VERSION);
	this->timer = timer;
	this->resolution = Setting::GetResolution();

	ZeroMemory(&presentParam, sizeof(presentParam));

	wnd = SGameWnd(GameWnd::Create(L"SpaghettiEngine"));

	presentParam.Windowed = TRUE; // thể hiện ở chế độ cửa sổ
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.BackBufferFormat = static_cast<D3DFORMAT>(colorFormat);
	presentParam.BackBufferCount = 1;
	presentParam.BackBufferWidth = resolution.width;
	presentParam.BackBufferHeight = resolution.height;
	presentParam.hDeviceWindow = wnd->GetContentWndHandler();

	CreateResource();
}


void Graphics::Render()
{
	timeSinceLastFrame += timer->GetDeltaTime();
	if (timeSinceLastFrame < delayPerFrame)
	{
		renderBuffer.clear();
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

	renderDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(rgb[0], rgb[1], rgb[2]), 1.0f, 0);

	if (Begin() != 0)
	{
		/// <summary>
		/// Render here
		/// </summary>

		PSpriteRenderer renderScript;
		Vector3 position;
		size_t renderSize = renderBuffer.size();
		size_t cameraSize = cameraList.size();
		auto itRender = renderBuffer.begin();
		auto itCamera = cameraList.begin();

		PMatrix screenMatrix;
		PMatrix cameraMatrix;
		PMatrix renderMatrix;

		spriteHandler->Begin(ALPHABLEND);
		for (size_t camera = 0; camera < cameraSize; camera++)
		{
			itRender = renderBuffer.begin();
			cameraMatrix = (*itCamera)->GetMatrix();
			screenMatrix = (*itCamera)->GetScreenMatrix();
			
			if (screenMatrix)
			{
				for (size_t render = 0; render < renderSize; render++)
				{
					renderScript = (*itRender);

					spriteHandler->SetTransform(&renderScript->GetTransform());
					spriteHandler->Draw(
						renderScript->GetTexture(),
						&renderScript->GetSourceRect(),
						NULL,
						&renderScript->GetPosition(),
						NULL
					);
				}
			}
		}
		spriteHandler->End();

		if (!End())
			Reset();
	}

	renderDevice->Present(NULL, NULL, NULL, NULL);
}

HRESULT Graphics::Begin() noexcept
{
	return renderDevice->BeginScene();
}

bool Graphics::End()
{
	// device alive
	if (!isDeviceLost)
	{
		renderDevice->EndScene();
	}
	HRESULT hr = renderDevice->TestCooperativeLevel();
	if (hr != D3D_OK)
	{
		if (hr == D3DERR_DEVICELOST)
		{
			isDeviceLost = true;
			spriteHandler->OnLostDevice();
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
	HRESULT hr = renderDevice->TestCooperativeLevel();
	if (hr == D3DERR_DEVICENOTRESET)
	{
		if (SUCCEEDED(renderDevice->Reset(&presentParam)))
		{
			// reset success
			spriteHandler->OnResetDevice();
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
	UINT adapterCount = renderer->GetAdapterCount();
	for (UINT i = 0; i < adapterCount; i++)
	{
		if (renderer->GetAdapterMonitor(i) == monitor)
		{
			videoAdapter = i;
			break;
		}
	}

	UINT modeCount = renderer->GetAdapterModeCount(videoAdapter, format);
	for (UINT i = 0; i < modeCount; i++)
	{
		DisplayMode mode;
		if (renderer->EnumAdapterModes(videoAdapter, format, i, &mode) == D3D_OK)
		{
			adapterMode.push_back(mode);
		}
	}
}

Graphics::Graphics() noexcept
{
	ZeroMemory(&presentParam, sizeof(presentParam));
}

Graphics::~Graphics() noexcept
{
	if (renderer)
		renderer->Release();
	renderer = nullptr;
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
	case NOTAVAILABLE:
		return L"File is not available";
	case OUTOFVIDEOMEMORY:
		return L"Out of video memory(gpu)";
	case INVALIDCALL:
		return L"Invalid call";
	case INVALIDDATA:
		return L"Invalid data";
	case OUTOFMEMORY:
		return L"Out of memory(ram)";
	default:
		return L": ^)";
	}
}

const HRESULT Graphics::GraphicCodeException::GetErrorCode() noexcept
{
	return code;
}
