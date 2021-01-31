#pragma once
#include <Windows.h>
#include <string>

using std::string;

class KeyBoard
{
	// Add more key here to use.
	// Specifically make for game.
	enum Key
	{
		Attack,
		Dash,
		Jump,
		Left,
		Right,
		Up,
		Down
	};
	//Mark key to handle
	string keyCode = "czx";

};

