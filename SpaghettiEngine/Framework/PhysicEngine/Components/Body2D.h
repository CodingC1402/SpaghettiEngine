#pragma once
#include <memory>

class Body2D;
typedef std::weak_ptr<Body2D> WBody2D;
typedef std::shared_ptr<Body2D> SBody2D;

class Body2D 
{
public:
	static WBody2D GetDefaultBody();
protected:


	static SBody2D _defaultBody;
};