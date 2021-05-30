#include "GameWnd.h"
#include "Setting.h"

PGameWnd GameWnd::__instance = nullptr;


HWND GameWnd::GetContentWndHandler()
{
	return contentWnd->GetHwnd();
}

PGameWnd GameWnd::Create(const wchar_t* name)
{
	Size resolution = Setting::GetResolution();
	return new GameWnd(resolution.width, resolution.height, name);
}

GameWnd::GameWnd(int width, int height, const wchar_t* name) : Window(width, height, name)
{
	contentWnd = Window::Create(width, height, L"", this, 0, 0);
	contentWnd->Show();
	this->SetBGBrush(0, 0, 0);
}

void GameWnd::OnSizeChanged(UINT width, UINT height)
{
	Window::OnSizeChanged(width, height);
	
	if (isFullScreen)
	{
		Size resolution = Setting::GetResolution();
		bool isPixelPerfect = Setting::IsResolutionPixelPerfect();
		float ratioW = width / (float)resolution.width;
		float ratioH = height / (float)resolution.height;
		if (isPixelPerfect)
		{
			ratioW = std::floorf(ratioW);
			ratioH = std::floorf(ratioH);
		}

		int contentWidth = width;
		int contentHeight = height;
		int contentPosX;
		int contentPosY;
		if (ratioW > ratioH)
		{
			contentHeight = std::round(static_cast<double>(resolution.height) * ratioH);
			contentWidth =  std::round(static_cast<double>(resolution.width ) * ratioH);
		}
		else
		{
			contentWidth =  std::round(static_cast<double>(resolution.width ) * ratioW);
			contentHeight = std::round(static_cast<double>(resolution.height) * ratioW);
		}
		contentPosY = std::round(((long long)height - contentHeight) / 2.0);
		contentPosX = std::round(((long long)width - contentWidth) / 2.0);

		contentWnd->SetPos(contentPosX, contentPosY);
		contentWnd->SetWidth(contentWidth);
		contentWnd->SetHeight(contentHeight);
	}
	else
	{
		Size resolution = Setting::GetResolution();
		contentWnd->SetPos(0, 0);
		contentWnd->SetWidth(resolution.width);
		contentWnd->SetHeight(resolution.height);
	}
}
