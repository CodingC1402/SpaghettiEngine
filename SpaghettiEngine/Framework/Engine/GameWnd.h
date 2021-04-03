#pragma once
#include "Window.h"

typedef class GameWnd* PGameWnd;
typedef std::shared_ptr<GameWnd> SGameWnd;
typedef std::unique_ptr<GameWnd> UGameWnd;

class GameWnd : public Window
{
public:
	HWND GetContentWndHandler();
	static PGameWnd Create(const wchar_t* name);
protected:
	GameWnd(int width, int height, const wchar_t* name);
	virtual void OnSizeChanged(UINT width, UINT height) override;
protected:
	PWindow contentWnd;

	static PGameWnd __instance;
};