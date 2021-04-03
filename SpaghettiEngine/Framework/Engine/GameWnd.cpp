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
	
	Size resolution = Setting::GetResolution();
	double ratioW = width / (double)resolution.width;
	double ratioH = height / (double)resolution.height;
	int contentWidth;
	int contentHeight;
	int contentPosX;
	int contentPosY;
	if (ratioW > ratioH)
	{
		contentHeight = height;
		contentWidth = (int)(resolution.width * ratioH + 0.5);
		contentPosY = 0;
		contentPosX = (int)(((long long)width - contentWidth) / 2.0 + 0.5);
	}
	else
	{
		contentWidth = width;
		contentHeight = (int)(resolution.height * ratioW + 0.5);
		contentPosY = (int)(((long long)height - contentHeight) / 2.0 + 0.5);
		contentPosX = 0;
	}
	contentWnd->SetPos(contentPosX, contentPosY);
	contentWnd->SetWidth(contentWidth);
	contentWnd->SetHeight(contentHeight);
}
