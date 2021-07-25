#include "DistanceAnimator.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "Texture.h"

REGISTER_FINISH(DistanceAnimator, Render2DScriptBase) {}

void DistanceAnimator::Draw(PCamera cam)
{
	Graphics::SetSpriteTransform(cam->GetMatrix(GetWorldMatrix()));
	Graphics::DrawSprite(
		_currentSprite,
		_currentSprite->GetCenter()
	);
}

void DistanceAnimator::Load(nlohmann::json& input)
{
	_distancePerFrame = input[Fields::DistanceAnimator::_distance].get<float>();

	STexture _texture;
	for (auto& sprite : input[Fields::DistanceAnimator::_sprites])
	{
		_texture = TextureContainer::GetInstance()->GetResource(sprite[Fields::DistanceAnimator::_texture].get<CULL>());
		_sprites.push_back(_texture->GetSprite(sprite[Fields::DistanceAnimator::_index].get<unsigned>()));
	}
	_currentSprite = _sprites.front();
}

void DistanceAnimator::OnStart()
{
	_orginalPos = GetWorldTransform();
}

void DistanceAnimator::OnFixedUpdate()
{
	auto currentPos = GetWorldTransform();
	float distance = currentPos.x - _orginalPos.x;

	unsigned size = static_cast<unsigned>(_sprites.size());
	auto index = (size + static_cast<int>(distance / _distancePerFrame) % size) % size;
	_currentSprite = _sprites[index];
}

ScriptBase* DistanceAnimator::Clone() const
{
	auto clone = dynamic_cast<DistanceAnimator*>(Render2DScriptBase::Clone());

	clone->_distancePerFrame = _distancePerFrame;
	clone->_orginalPos = _orginalPos;
	clone->_sprites = _sprites;
	clone->_currentSprite = _currentSprite;

	return clone;
}
