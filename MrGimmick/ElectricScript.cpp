#include "ElectricScript.h"
#include "GameTimer.h"
#include "FieldNames.h"
#include "Graphics.h"
#include "Setting.h"
#include "SMath.h"

REGISTER_FINISH(ElectricScript, Render2DScriptBase) {}

void ElectricScript::OnStart()
{
	_currentSprite = _elcAnim->GetSpriteOfFrame(0); // Get First frame
	_runningField = _animator->GetField<bool>("IsRunning");
}

void ElectricScript::OnUpdate()
{
	if (!_runningField.lock()->GetValue())
		Graphics::DrawSprite(_currentSprite, GetWorldTransform());
}

void ElectricScript::Draw(PCamera script)
{
	//auto worldMatrix = Matrix4::GetDiagonalMatrix();
	//auto gameObjectWorldMatrix = GetWorldMatrix();
	//worldMatrix._41 = gameObjectWorldMatrix._41;
	//worldMatrix._42 = gameObjectWorldMatrix._42;
	//
	//auto transform = script->GetMatrix(worldMatrix);
	//Graphics::SetSpriteTransform(transform);
	//
	//Graphics::DrawSprite(_currentSprite, _currentSprite->GetCenter());
}

void ElectricScript::Load(nlohmann::json& input)
{
	_elcAnim = AnimationContainer::GetInstance()->GetResource(input["ElectricAnim"].get<CULL>());

	Render2DScriptBase::Load(input);
}

PScriptBase ElectricScript::Clone() const
{
	auto clone = dynamic_cast<ElectricScript*>(Render2DScriptBase::Clone());

	clone->_counter = _counter;

	clone->_elcAnim = _elcAnim;
	clone->_currentSprite = _currentSprite;

	return clone;
}