#include "GameObj.h"

void GameObj::Update()
{
	for (int i = 0; i < scripts.size(); i++)
	{
		scripts[i]->Update();
	}
}

const char* GameObj::GetTag()
{
	return tag.c_str();
}

void GameObj::SetTag(const char* tag)
{
	this->tag = tag;
}

const char* GameObj::GetName()
{
	return name.c_str();
}

void GameObj::SetName(const char* name)
{
	this->name = name;
}
