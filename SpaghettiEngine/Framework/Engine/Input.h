#pragma once
#include "CornWnd.h";
#include <string>
#include <memory>

using std::string;

class Input
{
	enum KeyState
	{

	};
	string name;
	
};

typedef std::shared_ptr<Input> SInput;