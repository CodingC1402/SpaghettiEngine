#include "Graphics.h"
#include "Monitor.h"
#include "Setting.h"
#include "json.hpp"
#include "GraphicsMath.h"
#include "Render2DScriptBase.h"
#include "Setting.h"
#include "Sprite.h"
#include <fstream>
#include <DirectXMath.h>

#ifdef _DEBUG
#include "Debug.h"
#endif

PGraphics Graphics::__instance = nullptr;

Graphics::GraphicException::GraphicException(const int line, const char* file, std::wstring description) noexcept
	: 
	CornDescriptionException(line, file, std::move(description))
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

void Graphics::LoadTexture(PImage& rTexture, const std::string& path, const Color &keyColor)
{
	const std::wstring wPath = StringConverter::StrToWStr(path);
	ImageInfo info;

	HRESULT result = D3DXGetImageInfoFromFile(wPath.c_str(), &info);

	if (result != D3D_OK)
		throw GRAPHICS_EXCEPT_CODE(result);

	const auto gfx = __instance;
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
		nullptr,
		&rTexture
		);

	if (result != D3D_OK)
		throw GRAPHICS_EXCEPT_CODE(result);
}

void Graphics::AddRender2D(PRender2DScriptBase renderScript)
{
	std::lock_guard guard(__instance->_renderLock);
	__instance->_renderBuffer2D[renderScript->GetDrawLayer()].emplace_back(renderScript);
}

void Graphics::RemoveRender2D(PRender2DScriptBase renderScript)
{
	std::lock_guard guard(__instance->_renderLock);
	__instance->_renderBuffer2D[renderScript->GetDrawLayer()].remove(renderScript);
}

void Graphics::SetSpriteTransform(Matrix& matrix)
{
	if (Setting::IsWorldPointPixelPerfect())
	{
		matrix._41 = std::round(matrix._41);
		matrix._42 = std::round(matrix._42);
	}
	GetInstance()->spriteHandler->SetTransform(&matrix);
}

void Graphics::DrawSprite(const SSprite& sprite, const Vector3& center, const Vector3& position, const Color& color)
{
	RECT srcRect = sprite->GetSourceRect();
	GetInstance()->spriteHandler->Draw(
		sprite->GetSource()->GetImage(),
		&srcRect,
		&center,
		&position,
		color
	);
}

void Graphics::AddCamera(PCamera camera)
{
	std::lock_guard guard(__instance->_renderLock);
	__instance->cameraList.push_back(camera);
}

void Graphics::RemoveCamera(PCamera camera)
{
	std::lock_guard guard(__instance->_renderLock);
	__instance->cameraList.remove(camera);
}

void Graphics::SetActiveCamera(PCamera setCamera)
{
	std::lock_guard guard(__instance->_renderLock);
	__instance->cameraList.remove(setCamera);
	__instance->cameraList.emplace_front(setCamera);
}

void Graphics::ClearRenderBuffer2D()
{
	for (auto& layer : _renderBuffer2D)
		layer.clear();
}

void Graphics::ClearRenderBuffer()
{
	ClearRenderBuffer2D();
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

	if constexpr (Setting::IsDebugMode())
	{
		result = D3DXCreateFont(
		renderDevice,
		16, 8, 0, 0, 0, 0, 0, 0, 0, L"Calibri", &fpsFont
		);
		if (FAILED(result))
			throw GRAPHICS_EXCEPT_CODE(result);	
	}
}

void Graphics::ReleaseResource()
{
	if (renderDevice)
		renderDevice->Release();
	if (spriteHandler)
		spriteHandler->Release();
	renderDevice = nullptr;
}

void Graphics::FullScreen()
{
	if (isFullScreen)
		return;

	isFullScreen = true;
	wnd->ChangeWindowMode(true);
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

PCamera Graphics::GetActiveCamera()
{
	return *(__instance->cameraList.begin());
}

void Graphics::Init(const STimer& timer, const ColorFormat& colorFormat)
{
	if constexpr (Setting::IsDebugMode())
	{
		fpsTimer->Start();
		fpsRect.left = 3;
		fpsRect.top = 3;
		fpsRect.right = 50;
		fpsRect.bottom = 20;
	}

	if (const float fps = Setting::GetFps(); fps <= 0)
		delayPerFrame = 0;
	else
		delayPerFrame = 1 / fps;

	renderer = Direct3DCreate9(D3D_SDK_VERSION);
	this->timer = timer;
	this->resolution = Setting::GetResolution();

	ZeroMemory(&presentParam, sizeof(presentParam));

	wnd = SGameWnd(GameWnd::Create(L"SpaghettiEngine"));

	presentParam.Windowed = TRUE;
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.BackBufferFormat = static_cast<D3DFORMAT>(colorFormat);
	presentParam.BackBufferCount = 1;
	presentParam.BackBufferWidth = resolution.width;
	presentParam.BackBufferHeight = resolution.height;
	presentParam.hDeviceWindow = wnd->GetContentWndHandler();

	isPixelPerfect = Setting::IsWorldPointPixelPerfect();

	CreateResource();
}


void Graphics::Render()
{
	if (cameraList.empty())
		return;

	timeSinceLastFrame += timer->GetDeltaTime();
	if (timeSinceLastFrame < delayPerFrame)
		return;

	if constexpr(Setting::IsDebugMode())
	{
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
	}// For RGB background

	if (Begin() != 0)
	{
		timeSinceLastFrame -= delayPerFrame * static_cast<int>(timeSinceLastFrame / delayPerFrame);

		if constexpr  (Setting::IsDebugMode())
			renderDevice->Clear(0, nullptr, D3DCLEAR_TARGET, XRGB(rgb[0], rgb[1], rgb[2]), 1.0f, 0);
		else
			renderDevice->Clear(0, nullptr, D3DCLEAR_TARGET, BLACK, 1.0f, 0);

		if (_renderLock.try_lock())
		{
			spriteHandler->Begin(ALPHABLEND);
			const auto cameraScript = *cameraList.begin();
			for (auto& layer : _renderBuffer2D)
				for (auto renderScript2D = layer.begin(); renderScript2D != layer.end(); ++renderScript2D)
					(*renderScript2D)->Draw(cameraScript);
			_renderLock.unlock();
		}
		
		if constexpr (Setting::IsDebugMode())
		{
			UpdateFPS();
			std::wostringstream os;
			os << std::floor(fps + 0.5) << std::endl;
			const std::wstring str = os.str();

			const auto temp = GraphicsMath::NewMatrix();
			temp->_11 = 1;
			temp->_22 = 1;
			temp->_33 = 1;
			temp->_44 = 1;
			
			spriteHandler->SetTransform(temp);
			fpsFont->DrawTextW(
				spriteHandler,
				str.c_str(),
				str.size(),
				&fpsRect,
				DT_CHARSTREAM,
				MAGENTA
			);
			delete temp;
		}
		spriteHandler->End();
		
		if (!End())
			Reset();

		renderDevice->Present(nullptr, nullptr, nullptr, nullptr);
	}
}

HRESULT Graphics::Begin() const noexcept
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
	if (const HRESULT hr = renderDevice->TestCooperativeLevel(); hr != D3D_OK)
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
	if (renderDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
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
	const auto monitor = Monitor::GetCurrentMonitor(wnd->GetHwnd());
	const auto format = static_cast<D3DFORMAT>(colorFormat);
	adapterMode.clear();
	const UINT adapterCount = renderer->GetAdapterCount();
	for (UINT i = 0; i < adapterCount; i++)
	{
		if (renderer->GetAdapterMonitor(i) == monitor)
		{
			videoAdapter = i;
			break;
		}
	}

	const UINT modeCount = renderer->GetAdapterModeCount(videoAdapter, format);
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
	_renderBuffer2D = std::move(std::vector<std::list<PRender2DScriptBase>>(32));
}
Graphics::~Graphics() noexcept
{
	if (renderer)
		renderer->Release();
	renderer = nullptr;
	ReleaseResource();
}
#pragma region  Exception
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
	os << "[Description] " << Translate() << std::endl;
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
HRESULT Graphics::GraphicCodeException::GetErrorCode() noexcept
{
	return code;
}
#pragma endregion