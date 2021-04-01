#include "Game.h"

void Game::Update()
{
	if (InputSystem::GetInput("Left")->Check())
	{
		App::CallQuit();
	}
}
