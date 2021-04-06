#include "GameObj.h"

void GameObj::Start()
{

}

void GameObj::Update()
{
	int i = scripts.size();
	auto script = scripts.begin();
	while (i > 0)
	{
		(*script)->Update();
		std::advance(script, 1);
		i--;
	}
}

void GameObj::End()
{
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

GameObj::~GameObj()
{
	int size = children.size();
	auto iterator = children.begin();

}
