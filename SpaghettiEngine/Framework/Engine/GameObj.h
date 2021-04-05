#pragma once
#include <memory>
#include "Sprite.h"
#include "Animation.h"

typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;
typedef std::unique_ptr<GameObj> UGameObjl;

class GameObj
{
	SSprite sprite;
	SAnimation animation;
};