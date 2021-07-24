#pragma once
#include "Vector3.h"
#include "Macros.h"
#include "Color.h"
#include <set>
#include <unordered_map>

CLASS_FORWARD_DECLARATION(UIComponent);
CLASS_FORWARD_DECLARATION(Canvas);

class UIComponent
{
	friend class UIFactory;
	friend class Canvas;
public:
	[[nodiscard]] const Vector3& GetPosition() const noexcept;
	[[nodiscard]] const float& GetWidth() const noexcept;
	[[nodiscard]] const float& GetHeight() const noexcept;
	[[nodiscard]] const Color& GetColor() const noexcept;

	void SetPosition(const Vector3& pos) noexcept;
	void SetWidth(const float& width) noexcept;
	void SetHeight(const float& height) noexcept;
	void SetColor(const Color& color) noexcept;

	void AddChild(UIComponent* component);
	void RemoveChild(UIComponent* component);
	
	virtual void Load(nlohmann::json& input);

	[[nodiscard]] const std::set<UIComponent*>& GetChildren() const noexcept;
	[[nodiscard]] const std::string& GetType() const noexcept;
	[[nodiscard]] const std::string& GetName() const noexcept;

	[[nodiscard]] UIComponent* Clone(PCanvas canvas);

	virtual void Draw(Vector3 position);
private:
	void SetType(const std::string& className) noexcept;
	void SetName(const std::string& componentName) noexcept;
	void SetCanvas(PCanvas canvas);
private:
	std::set<UIComponent*> _children;
	PCanvas _canvas = nullptr;

	Vector3 _position;
	float _width = 0.0f;
	float _height = 0.0f;
	Color _color = 0x00000000;

	std::string _componentName;
	const std::string* _typeStr = nullptr;
};

class UIFactory
{
public:
	static std::shared_ptr<UIComponent> Create(const std::string& className);
	template <typename T>
	static void Register(const std::string& className);
private:
	template <typename T>
	static std::shared_ptr<UIComponent> CreateFunction();
private:
	static inline std::unordered_map<std::string, std::shared_ptr<UIComponent>(*)()> _createFunction;
};

template<typename T>
class UIFactoryRegistration
{
public:
	UIFactoryRegistration(const std::string& className);
	UIFactoryRegistration(const char* className);
};

#define REGISTER_UI_COMPONENT_START(CLASSNAME) private: static UIFactoryRegistration<CLASSNAME> _UI_REGISTRATION_KEY;
#define REGISTER_UI_COMPONENT_FINISH(CLASSNAME) UIFactoryRegistration<CLASSNAME> CLASSNAME::_UI_REGISTRATION_KEY(#CLASSNAME);

template<typename T>
inline UIFactoryRegistration<T>::UIFactoryRegistration(const std::string& className)
{
	UIFactory::Register<T>(className);
}

template<typename T>
inline UIFactoryRegistration<T>::UIFactoryRegistration(const char* className)
{
	UIFactory::Register<T>(className);
}

template<typename T>
inline std::shared_ptr<UIComponent> UIFactory::CreateFunction()
{
	return std::dynamic_pointer_cast<UIComponent>(std::make_shared<T>());
}

template<typename T>
inline void UIFactory::Register(const std::string& className)
{
	_createFunction.emplace(className, &CreateFunction<T>);
}
