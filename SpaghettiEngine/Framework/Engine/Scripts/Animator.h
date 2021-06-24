#pragma once
#include "Render2DScriptBase.h"
#include "AnimationTree.h"

class Animator : public Render2DScriptBase
{
public:
	Animator(PScene owner);
	virtual void OnUpdate() override;
	virtual void Load(nlohmann::json& inputObject) override;
	virtual void Draw(PCamera camera) override;

	template<typename T>
	void SetFieldValue(const std::string& fieldName, T value);
	template<typename T>
	T GetFieldValue(const std::string& fieldName);

	PScriptBase Clone() const override;
protected:
	SAnimationTree _tree;
private:
	REGISTER_START(Animator);
};

template<typename T>
inline void Animator::SetFieldValue(const std::string& fieldName, T value)
{
	_tree->GetBlackBoard().lock()->SetValue(fieldName, value);
}

template<typename T>
inline T Animator::GetFieldValue(const std::string& fieldName)
{
	return _tree->GetBlackBoard().lock()->GetValue(fieldName);
}
