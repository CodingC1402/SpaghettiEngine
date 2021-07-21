#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>
#include <sstream>
#include <fstream>
#include <mutex>

#include "CMessageBox.h"
#include "CornException.h"
#include "SMath.h"
#include "System.h"
#include "json.hpp"

//============================================================{Resource Base}=====================================================//
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
	Resource() = default;
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

//==========================================================={Container template}======================================================//
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
			bool& IsLoaded() noexcept;
			std::shared_ptr<T>& GetResource();
			std::string& GetPath();
		protected: 
			std::shared_ptr<T> _resource;
			std::string _path;
			bool _isLoaded = false;
		};
	public:
		ResourceList(Container* owner);
		void LoadEntriesList(const std::string& path);
		
		Entry& GetEntry(CULL& id);
		const char* GetContainerName() const;
		
		void UnloadEntry(CULL& id);
		void UnloadUnusedEntries();
		void UnloadEntries();
	protected:
		Container<T>* _owner;
		std::map<unsigned long long, Entry> _entries;
	};
public:
	Container();
	
	Container(const Container&) = delete;
	Container(const Container&&) = delete;
	Container& operator= (const Container&) = delete;
	Container& operator= (const Container&&) = delete;

	std::shared_ptr<T>& GetResource(CULL& id);

	void UnloadResource(CULL& id);
	void UnloadUnusedResources();
	void UnloadResources();

	static const char* GetName();
	static Container* GetInstance();
protected:
	virtual ~Container() = default;

	void LoadEntries(const std::string& path);
protected:
	std::string _name;
	ResourceList _resources;

	std::recursive_mutex _resourceLock;

	static Container<T>* _instance;
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

#define CONTAINER_REGISTER(containerName, resourceName) template<> Container<resourceName>* Container<resourceName>::_instance = new containerName()
#define RESOURCE_NAME(resourceName) #resourceName
#define CONTAINER_EXCEPT(containerName, description) ContainerException(__LINE__,__FILE__,containerName,description)

//======================================================={inline def}=================================================================//
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
	if (!_isLoaded)
		return;
	_resource.reset();
	_isLoaded = false;
}

template <typename T>
void Container<T>::ResourceList::Entry::UnloadIfUnused()
{
	if (_resource.use_count() > 1 || !_isLoaded || !_resource->IsResourceUnused())
		return;
	_resource.reset();
	_isLoaded = false;
}

template <typename T>
bool& Container<T>::ResourceList::Entry::IsLoaded() noexcept
{
	return _isLoaded;
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
void Container<T>::ResourceList::LoadEntriesList(const std::string& path)
{
	try
	{
		using namespace nlohmann;
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::ostringstream os;
			os << "[Exception] Entries file ";
			os << path.c_str();
			os << " doesn't exist";
			throw CONTAINER_EXCEPT(GetContainerName(), os.str());
		}

		try
		{
			json jsonFile;
			file >> jsonFile;

			for (const auto& entry : jsonFile)
			{
				_entries.emplace(entry["ID"].get<ULL>(), entry["Path"].get<std::string>());
			}
		}
		catch (const std::exception&)
		{
			std::ostringstream os;
			os << "[Type] Entry list error";
			os << "[Exception] The format of the entry list is wrong";
			throw CONTAINER_EXCEPT(_owner->_name, os.str());
		}
	}
	catch (const CornException& e)
	{
		CMessageBox msg(e.What(), e.GetType(), CMessageBox::Flags::OK | CMessageBox::Flags::ICONEXCLAMATION);
		System::Exit(-1);
	}
}

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
	if (!entry->second.IsLoaded())
		entry->second.Load();
	return entry->second;
}

template <typename T>
const char* Container<T>::ResourceList::GetContainerName() const
{
	return _owner->GetName();
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
		entry.second.UnloadIfUnused();
}

template <typename T>
void Container<T>::ResourceList::UnloadEntries()
{
	for (auto& entry : _entries)
		entry.second.Unload();
}

template <typename T>
std::shared_ptr<T>& Container<T>::GetResource(CULL& id)
{
	std::lock_guard guard(_resourceLock);
	return _resources.GetEntry(id).GetResource();
}

template <typename T>
void Container<T>::UnloadResource(CULL& id)
{
	std::lock_guard guard(_resourceLock);
	_resources.GetEntry(id).Unload();
}

template <typename T>
void Container<T>::UnloadUnusedResources()
{
	std::lock_guard guard(_resourceLock);
	_resources.UnloadUnusedEntries();
}

template <typename T>
void Container<T>::UnloadResources()
{
	std::lock_guard guard(_resourceLock);
	_resources.UnloadEntries();
}

template <typename T>
const char* Container<T>::GetName()
{
	return GetInstance()->_name.c_str();
}

template <typename T>
Container<T>* Container<T>::GetInstance()
{
	if (!_instance)
		_instance = new Container<T>();
	return _instance;
}

template <typename T>
Container<T>::Container()
	:
	_resources(this)
{
		
}

template <typename T>
void Container<T>::LoadEntries(const std::string& path)
{
	_resources.LoadEntriesList(path);
}
#pragma endregion
