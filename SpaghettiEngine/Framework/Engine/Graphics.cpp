#include "Graphics.h"
#include "Monitor.h"
#include "Setting.h"
#include "json.hpp"
#include "GraphicsMath.h"
#include <fstream>
#include <DirectXMath.h>

#ifdef _DEBUG
#include "Debug.h"
#endif

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

void Graphics::LoadTexture(PDx9Texture& rTexture, const std::string& path, const Color &keyColor)
{
	HRESULT result;
	std::wstring wPath = StringConverter::StrToWStr(path);
	ImageInfo info;

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
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParam,
		&renderDevice
	);

	HRESULT result = D3DXCreateSprite(renderDevice, &spriteHandler);
	if (FAILED(result))
		throw GRAPHICS_EXCEPT_CODE(result);

	if (FAILED(result))
		throw GRAPHICS_EXCEPT_CODE(result);

	if (!renderer)
		throw GRAPHICS_EXCEPT(L"Can't initialize directX properly");
	if (!renderDevice)
		throw GRAPHICS_EXCEPT(L"Can't initialize driectXDev, and there is no error code for this so... good luck fixing this ヽ(￣ω￣(。。 )ゝ ");

#ifdef _DEBUG
	result = D3DXCreateFont(
		renderDevice,
		15, 10, 0, 0, 0, 0, 0, 0, 0, L"Calibri", &fpsFont
	);
	if (FAILED(result))
		throw GRAPHICS_EXCEPT_CODE(result);
#endif // DEBUG

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

void Graphics::Init(STimer timer, ColorFormat colorFormat)
{
#ifdef _DEBUG // For counting fps
	fpsTimer->Start();
	fpsRect.left = 3;
	fpsRect.top = 3;
	fpsRect.right = 50;
	fpsRect.bottom = 20;
#endif

	int fps = Setting::GetFps();
	fps = fps;
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

	isPixelPerfect = Setting::IsPixelPerfect();

	CreateResource();
}


void Graphics::Render()
{
	if (cameraList.size() == 0)
		return;

	timeSinceLastFrame += timer->GetDeltaTime();
	if (timeSinceLastFrame < delayPerFrame)
	{
		renderBuffer.clear();
		return;
	}

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

	if (Begin() != 0)
	{
		timeSinceLastFrame -= delayPerFrame * static_cast<int>(timeSinceLastFrame / delayPerFrame);
		renderDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(rgb[0], rgb[1], rgb[2]), 1.0f, 0);

		PMatrix cameraMatrix;
		auto camera = cameraList.begin();
		cameraMatrix = (*camera)->GetMatrix();

		spriteHandler->Begin(ALPHABLEND);

		Vector3 screenPosition;
		for (const auto& renderScript : renderBuffer)
		{
			GraphicsMath::TransformVector3(&screenPosition, renderScript->GetPosition(), cameraMatrix);
			if (isPixelPerfect)
				GraphicsMath::RoundVector3(&screenPosition);

			spriteHandler->SetTransform(renderScript->GetTransform());
			spriteHandler->Draw(
				renderScript->GetTexture(),
				renderScript->GetSourceRect(),
				renderScript->GetCenter(),
				&screenPosition,
				D3DCOLOR_XRGB(255, 255, 255)
			);
		}

#ifdef _DEBUG // For counting fps
		if (cameraList.size() > 1)
			Debug::Log(L"there are two or more camera in a scene");

		UpdateFPS();
		std::wostringstream os;
		os << (int)fps << std::endl;
		std::wstring str = os.str();

		fpsFont->DrawTextW(
			spriteHandler,
			str.c_str(),
			str.size(),
			&fpsRect,
			DT_CHARSTREAM,
			XRGB(255, 255, 255)
		);
#endif
		spriteHandler->End();

		renderBuffer.clear();

		if (!End())
			Reset();

		renderDevice->Present(NULL, NULL, NULL, NULL);
	}
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
