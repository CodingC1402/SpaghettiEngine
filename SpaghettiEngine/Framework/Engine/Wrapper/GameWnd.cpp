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
			ratioW = (int)ratioW;
			ratioH = (int)ratioH;
		}

		int contentWidth = width;
		int contentHeight = height;
		int contentPosX;
		int contentPosY;
		if (ratioW > ratioH)
		{
			contentHeight = (int)(resolution.height * ratioH + 0.5);
			contentWidth = (int)(resolution.width * ratioH + 0.5);
		}
		else
		{
			contentWidth = (int)(resolution.width * ratioW + 0.5);;
			contentHeight = (int)(resolution.height * ratioW + 0.5);
		}
		contentPosY = (int)(((long long)height - contentHeight) / 2.0 + 0.5);;
		contentPosX = (int)(((long long)width - contentWidth) / 2.0 + 0.5);

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
