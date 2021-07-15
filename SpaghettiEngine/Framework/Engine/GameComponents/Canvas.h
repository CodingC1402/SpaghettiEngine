#pragma once
#include "ScriptBase.h"
#include "UIComponent.h"
#include <unordered_map>

class Canvas : public ScriptBase
{
	friend class UIComponent;
	friend class Graphics;
public:
	void Load(nlohmann::json& input) override;
	[[nodiscard]] UIComponent* GetComponent(const std::string& name) noexcept;
private:
	void Draw();

	void AddComponent(SUIComponent component);
	void RemoveComponent(SUIComponent component);
	void RemoveComponent(const std::string& name);
private:
	std::unordered_map<std::string, SUIComponent> _components;
	UIComponent _root;

	REGISTER_START(Canvas);
};