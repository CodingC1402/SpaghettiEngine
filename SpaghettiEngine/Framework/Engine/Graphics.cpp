#include "Graphics.h"

PGraphics Graphics::__instance = nullptr;

Graphics::GraphicException::GraphicException(int line, const char* file, const wchar_t* discription) noexcept : CornException(line, file)
{
	this->discription = discription;
}

const wchar_t* Graphics::GraphicException::GetType() const noexcept
{
	return L"∑(O_O;) Graphic Exception";
}

const wchar_t* Graphics::GraphicException::What() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::wstring Graphics::GraphicException::GetErrorString() const noexcept
{
	return discription;
}

PGraphics Graphics::GetInstance()
{
	if (!__instance)
		__instance = new Graphics();
	return __instance;
}

void Graphics::Init(STimer timer, int fps)
{
	if (fps <= 0)
		delayPerFrame = 0;
	else
		delayPerFrame = 1 / static_cast<double>(fps);

	this->timer = timer;

	ZeroMemory(&dxpp, sizeof(dxpp));

	dxpp.Windowed = TRUE; // thể hiện ở chế độ cửa sổ
	dxpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dxpp.BackBufferFormat = D3DFMT_UNKNOWN;

	dx = Direct3DCreate9(D3D_SDK_VERSION);
	dx->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		Window::GetInstance()->GetHwnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&dxpp,
		&dxdev
	);

	if (!dx)
		throw GRAPHICS_EXCEPT(L"Can't initialize directX properly");
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

	if (dxdev->BeginScene() != 0)
	{
		/// <summary>
		/// Render here
		/// </summary>

		dxdev->EndScene();
	}

	dxdev->Present(NULL, NULL, NULL, NULL);
}

Graphics::Graphics() noexcept
{
	ZeroMemory(&dxpp, sizeof(dxpp));
}

Graphics::~Graphics() noexcept
{
	if (dx)
		dx->Release();
	if (dxdev)
		dxdev->Release();
}
