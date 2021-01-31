#pragma once
#include "Scene.h"

class Game
{
private:
	Scene *m_pScenes;
public:
	virtual int Update();
	virtual void Render();
};

