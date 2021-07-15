#include "Canvas.h"
#include "Graphics.h"

REGISTER_FINISH(Canvas, ScriptBase) {}

void Canvas::OnEnabled()
{
	Graphics::AddUIRender(this);
}

void Canvas::OnDisabled()
{
	Graphics::RemoveUIRender(this);
}

void Canvas::Load(nlohmann::json& input)
{
	_root.SetCanvas(this);
	_root.Load(input);
}

UIComponent* Canvas::GetComponent(const std::string& name) noexcept
{
	return _components[name].get();
}

void Canvas::Draw()
{
	_root.Draw(GetWorldTransform());
}

void Canvas::AddComponent(SUIComponent component)
{
	_components.emplace(component->GetName(), component);
}

void Canvas::RemoveComponent(SUIComponent component)
{
	_components.erase(component->GetName());
}

void Canvas::RemoveComponent(const std::string& name)
{
	_components.erase(name);
}