#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>
#include <sstream>
#include "ExMath.h"
#include "CornException.h"
#include "StringConverter.h"

#pragma region Resource base
class Resource
{
public:
	class ResourceException : public CornException
	{
	public:
		ResourceException(int line, const char* file, const std::string& descrpition, const std::string& resourceType);
		const wchar_t* What() const noexcept override;
		const wchar_t* GetType() const noexcept override;
	protected:
		std::string _description;
		std::string _resourceType;
	};
public:
	virtual ~Resource() = default;
	
	Resource(const Resource&) = delete;
	Resource(const Resource&&) = delete;
	Resource& operator= (const Resource&) = delete;
	Resource& operator= (const Resource&&) = delete;

	[[nodiscard]] virtual bool IsResourceUnused() const;
	virtual void Load(const std::string& path) = 0;
};

#define RESOURCE_LOAD_EXCEPTION(description, resourceType) Resource::ResourceException(__LINE__,__FILE__,description,#resourceType)
#pragma endregion

template<typename T>
class Container
{
protected:
	class ResourceList
	{
	public:
		class Entry
		{
		public:
			Entry(const std::string& path);
			void Load();
			void Unload();
			void UnloadIfUnused();
			std::shared_ptr<T>& GetResource();
			std::string& GetPath();
		protected:
			std::shared_ptr<T> _resource;
			std::string _path;
			bool _isLoaded = false;
		};
	public:
		ResourceList(Container* owner);
		
		Entry& GetEntry(CULL& id);
		
		void UnloadEntry(CULL& id);
		void UnloadUnusedEntries();
		void UnloadEntries();
	protected:
		Container* _owner;
		std::map<unsigned long long, Entry> _entries;
	};
public:
	Container(const Container&) = delete;
	Container(const Container&&) = delete;
	Container& operator= (const Container&) = delete;
	Container& operator= (const Container&&) = delete;

	std::shared_ptr<T>& GetResource(CULL& id);

	void UnloadResource(CULL& id);
	void UnloadUnusedResources();
	void UnloadResources();
	
	static Container* GetInstance();
protected:
	Container() = default;
	virtual ~Container() = default;
protected:
	std::string _name;
	ResourceList _resources;

	static Container* _instance;
};

class ContainerException : public CornException
{
public:
	ContainerException(int line, const char* file, const std::string& containerName, const std::string& description);
	const wchar_t* What() const noexcept override;
	const wchar_t* GetType() const noexcept override;
protected:
	std::string _containerName;
	std::string _description;
};

#define CONTAINER_REGISTER(resourceName) Container<resourceName>* Container<resourceName>::_instance = new Container<resourceName>()
#define RESOURCE_NAME(resourceName) #resourceName
#define CONTAINER_EXCEPT(containerName, description) ContainerException(__LINE__,__FILE__,containerName,description)

#pragma region Def
template <typename T>
Container<T>::ResourceList::Entry::Entry(const std::string& path)
	:
	_path(path)
{}

template <typename T>
void Container<T>::ResourceList::Entry::Load()
{
	_resource = std::make_shared<T>();
	_resource->Load(_path);
	_isLoaded = true;
}

template <typename T>
void Container<T>::ResourceList::Entry::Unload()
{
	_resource.reset();
	_isLoaded = false;
}

template <typename T>
void Container<T>::ResourceList::Entry::UnloadIfUnused()
{
	if (_resource->IsResourceUnused())
		return;
	_resource.reset();
	_isLoaded = false;
}

template <typename T>
std::shared_ptr<T>& Container<T>::ResourceList::Entry::GetResource()
{
	return _resource;
}

template <typename T>
std::string& Container<T>::ResourceList::Entry::GetPath()
{
	return _path;
}

template <typename T>
Container<T>::ResourceList::ResourceList(Container* owner)
	:
	_owner(owner)
{}

template <typename T>
typename Container<T>::ResourceList::Entry& Container<T>::ResourceList::GetEntry(CULL& id)
{
	auto entry = _entries.find(id);
	if (entry == _entries.end())
	{
		std::ostringstream os;
		os << "[Entry ID] " << id << std::endl;
		os << "[Exception] Entry id doesn't exist in the entry list" << std::endl;
		throw CONTAINER_EXCEPT(_owner->_name, os.str());
	}
	if (!entry->second._isLoaded)
		entry.Load();
	return entry;
}

template <typename T>
void Container<T>::ResourceList::UnloadEntry(CULL& id)
{
	_entries[id].Unload();
}

template <typename T>
void Container<T>::ResourceList::UnloadUnusedEntries()
{
	for (auto& entry : _entries)
		entry.second->UnloadIfUnused();
}

template <typename T>
void Container<T>::ResourceList::UnloadEntries()
{
	for (auto& entry : _entries)
		entry.second->Unload();
}

template <typename T>
std::shared_ptr<T>& Container<T>::GetResource(CULL& id)
{
	return _resources.GetEntry(id)._resource;
}

template <typename T>
void Container<T>::UnloadResource(CULL& id)
{
	_resources.GetEntry(id).Unload();
}

template <typename T>
void Container<T>::UnloadUnusedResources()
{
	_resources.UnloadUnusedEntries();
}

template <typename T>
void Container<T>::UnloadResources()
{
	_resources.UnloadEntries();
}

template <typename T>
Container<T>* Container<T>::GetInstance()
{
	if (!_instance)
		_instance = new Container<T>();
	return _instance;
}
#pragma endregion
