#include "Animator.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "Setting.h"
#include "LoadingJson.h"
#include "ScriptField.h"

REGISTER_FINISH(Animator, Render2DScriptBase) {}

void Animator::OnUpdate()
{
	_tree->Tick();
}

void Animator::Draw(PCamera camera)
{
	auto sprite = _tree->GetCurrentSprite();
	if (!sprite.use_count())
		return;

	Matrix4 transform = camera->GetMatrix(GetWorldMatrix());
	Graphics::SetSpriteTransform(transform);
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
		using Fields::Animator;

		std::string _treeFilePath = inputObject[Animator::GetAnimationTreeField()].get<std::string>();

		_tree = MAKE_SHARE_BT(AnimationTree);
		_tree->Load(_treeFilePath, inputObject[Animator::GetChangesField()]);
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
