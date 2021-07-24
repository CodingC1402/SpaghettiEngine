#include "Treasure.h"
#include "FieldNames.h"
#include "PlayerScoreBoard.h"
#include "GameTimer.h"
#include "Graphics.h"

REGISTER_FINISH(Treasure, Render2DScriptBase) {}

void Treasure::OnUpdate()
{
	_smallCounter += GameTimer::GetDeltaTime();
	_bigCounter += GameTimer::GetDeltaTime();

	_smallToBig->Advance(_smallIndex, _smallCounter);
	_bigToSmall->Advance(_bigIndex, _bigCounter);
}

void Treasure::OnEnd()
{
	PlayerScoreBoard::GetInstance()->IncreaseScore(_point);
}

void Treasure::Draw(PCamera cam)
{
	SSprite bigSprite = _bigToSmall->GetSpriteOfFrame(_bigIndex);
	SSprite smallSprite = _smallToBig->GetSpriteOfFrame(_smallIndex);

	Graphics::SetSpriteTransform(cam->GetMatrix(GetWorldMatrix()));
	Graphics::DrawSprite(
		_sprite,
		_sprite->GetCenter()
	);
	for (auto& center : _smallCenters)
	{
		Graphics::DrawSprite(
			smallSprite,
			smallSprite->GetCenter(),
			center
		);
	}
	for (auto& center : _bigCenters)
	{
		Graphics::DrawSprite(
			bigSprite,
			bigSprite->GetCenter(),
			center
		);
	}
}

void Treasure::Load(nlohmann::json& input)
{
	STexture texture = TextureContainer::GetInstance()->GetResource(input[Fields::Treasure::_texture].get<CULL>());
	_sprite = texture->GetSprite(input[Fields::Treasure::_index].get<unsigned>());
	_point = input[Fields::Treasure::_point].get<unsigned>();

	nlohmann::json smallJson = input[Fields::Treasure::_SToB];
	nlohmann::json bigJson = input[Fields::Treasure::_BToS];

	_smallToBig = AnimationContainer::GetInstance()->GetResource(smallJson[Fields::Treasure::_animation].get<CULL>());
	for (auto& pos : smallJson[Fields::Treasure::_positions])
		_smallCenters.push_back(pos);

	_bigToSmall= AnimationContainer::GetInstance()->GetResource(bigJson[Fields::Treasure::_animation].get<CULL>());
	for (auto& pos : bigJson[Fields::Treasure::_positions])
		_bigCenters.push_back(pos);
}

ScriptBase* Treasure::Clone() const
{
	auto clone = dynamic_cast<Treasure*>(ScriptBase::Clone());

	clone->_point = _point;
	clone->_sprite = _sprite;
	clone->_smallToBig = _smallToBig;
	clone->_bigToSmall = _bigToSmall;
	clone->_smallCenters = _smallCenters;
	clone->_bigCenters = _bigCenters;

	return clone;
}
