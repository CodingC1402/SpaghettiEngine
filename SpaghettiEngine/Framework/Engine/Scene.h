#pragma once

#include "CornException.h"
#include "ExMath.h"
#include <string>
#include <list>
#include <map>
#include <memory>
#include <json.hpp>

typedef class GameObj* PGameObj;
typedef class TileObj* PTileObj;
typedef class Scene* PScene;
typedef std::unique_ptr<Scene> UScene;
typedef std::shared_ptr<Scene> SScene;

using namespace nlohmann;
class Scene
{
	friend class SceneManager;
public:
	class SceneException : public CornException
	{
	public:
		SceneException(int line, const char* file, const std::string& description);
		const wchar_t* GetType() const noexcept override;
		const wchar_t* What() const noexcept override;
	protected:
		std::string _description;
	};

	typedef class BaseComponent
	{
	public:
		BaseComponent(PScene owner);
		virtual ~BaseComponent();

		BaseComponent(const BaseComponent&) = default;
		BaseComponent(const BaseComponent&&) = default;
		BaseComponent& operator=(const BaseComponent&) = default;
		BaseComponent& operator=(const BaseComponent&&) = default;

		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnEnd() {}
		
		virtual void OnDisabled() {}
		virtual void OnEnabled() {}
		
		virtual void Disable();
		virtual void Enable();
		virtual bool [[nodiscard]] IsDisabled();
		
		virtual void Load(nlohmann::json& input);
		virtual void Destroy();

		PScene GetOwner() const;
	protected:
		PScene _owner = nullptr;
		bool _isDisabled;
	} *PBaseComponent;
	typedef std::shared_ptr<BaseComponent> SBaseComponent;

public:
	// Only work in load
	SBaseComponent& GetComponent(CULL& id);
	void PromoteGameObjToRoot(PGameObj gameObj);
	void DemoteGameObjFromRoot(PGameObj gameObj);
	
	void Start();
	void Update();
	void End();
protected:
	Scene(const std::string& path);

	void Load();
	void Unload();
protected:
	std::string path;

	// Will update every loop
	std::list<PGameObj> _rootGameObjects;
	
	std::map<PBaseComponent, SBaseComponent> _genericComponents;
	std::map<CULL, PBaseComponent>* _tempComponentContainer;
};

#define SCENE_EXCEPTION(description) Scene::SceneException(__LINE__,__FILE__,description)