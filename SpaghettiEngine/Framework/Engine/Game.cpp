#include "Game.h"

void Game::Update()
{
	if (InputSystem::GetInput("Left")->Check())
	{
		App::CallQuit();
	}
	if (InputSystem::GetInput("Up")->Check())
	{
		Graphics::ToFullScreenMode();
	}
	if (InputSystem::GetInput("Down")->Check())
	{
		Graphics::ToWindowMode();
	}
}
