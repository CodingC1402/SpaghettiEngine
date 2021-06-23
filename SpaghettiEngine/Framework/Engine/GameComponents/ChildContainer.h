#pragma once 

#include <list>
#include <string>
#include <vector>
#include "Macros.h"

CLASS_FORWARD_DECLARATION(GameObj);

class ChildContainer
{
public:
	PGameObj GetGameObject(const std::string& name);
	std::vector<PGameObj> GetGameObjects(const std::string& name);
protected:
	std::list<PGameObj> _container;
};