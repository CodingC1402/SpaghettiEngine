#include "Body2D.h"

SBody2D Body2D::_defaultBody = std::make_shared<Body2D>();

WBody2D Body2D::GetDefaultBody()
{
	return _defaultBody;
}
