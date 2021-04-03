#include "App.h"

//Entry point
int WINAPI wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow
)
{
	PApp app = App::GetInstance();
	//app->ShowExtraInfo();
	BOOL result = app->Go();
	return result;
}