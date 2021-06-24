#include "Animator.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "Setting.h"
#include "LoadingJson.h"

REGISTER_FINISH(Animator);

Animator::Animator(PScene owner)
	:
	Render2DScriptBase(owner)
{}

void Animator::OnUpdate()
{
	_tree->Tick();
}

void Animator::Draw(PCamera camera)
{
	Matrix4 transform = camera->GetMatrix(GetWorldMatrix());
	Graphics::SetSpriteTransform(transform);

	auto sprite = _tree->GetCurrentSprite();
	Graphics::DrawSprite(sprite, sprite->GetCenter());
}

PScriptBase Animator::Clone() const
{
	auto animClone = dynamic_cast<Animator*>(Render2DScriptBase::Clone());

	animClone->_tree = std::dynamic_pointer_cast<AnimationTree>(_tree->Clone());
	animClone->_drawLayer = _drawLayer;

	return animClone;
}

void Animator::Load(nlohmann::json& inputObject)
{
	try
	{
		using LoadingJson::Field;

		std::string _treeFilePath = inputObject[Field::behaviorTreeField].get<std::string>();

		_tree = std::make_shared<AnimationTree>();
		_tree->Load(_treeFilePath);
	}
	catch (const CornException& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, e.What());
	}
	catch(const std::exception& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, StringConverter::StrToWStr(e.what()));
	}
	Render2DScriptBase::Load(inputObject);
}
