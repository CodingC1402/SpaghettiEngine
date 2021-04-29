#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>
#include <sstream>
#include "CornException.h"
#include "StringConverter.h"

class Resource
{
public:
	Resource(const std::string& path);
	virtual ~Resource() = default;
	
	Resource(const Resource&) = delete;
	Resource(const Resource&&) = delete;
	Resource& operator= (const Resource&) = delete;
	Resource& operator= (const Resource&&) = delete;

	[[nodiscard]] virtual std::string GetPath() const;
	[[nodiscard]] virtual unsigned long long GetHash() const;
	[[nodiscard]] virtual bool IsResourceUnused() const;
	virtual void Load() = 0;
protected:
protected:
	std::string _path;
	unsigned long long _hash;
};

template<typename T>
class Container
{
public:
	Container(const Container&) = delete;
	Container(const Container&&) = delete;
	Container& operator= (const Container&) = delete;
	Container& operator= (const Container&&) = delete;

	static std::shared_ptr<T> GetResource(int index);
	static std::shared_ptr<T> GetResource(const std::string& path);
	static std::shared_ptr<T> LoadResource(const std::string& path);

	static Container* GetInstance();
protected:
	static void RemoveResources(const std::string& path);
	static void ClearUnusedResources();
	static void ClearResources();
	
	Container() = default;
	virtual ~Container() = default;
protected:
	static std::string _name;
	inline static std::map<unsigned long long, std::shared_ptr<T>> _resources;

	inline static Container* _instance;
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

#define CONTAINER_EXCEPT(containerName, description) ContainerException(__LINE__,__FILE__,containerName,description)
#define CONTAINER_REGISTER_NAME(classType, resourceType) std::string Container<resourceType>::_name = #classType

#pragma region Def
template <typename T>
std::shared_ptr<T> Container<T>::GetResource(int index)
{
	if (index >= _resources.size() || index < 0)
		return std::shared_ptr<T>();
	return _resources[index];
}

template <typename T>
std::shared_ptr<T> Container<T>::GetResource(const std::string& path)
{
	const unsigned long long findHash = StringConverter::HashStr(path);
	auto rIt = _resources.find(findHash);
	if (rIt == _resources.end())
		return  LoadResource(path);
	return rIt->second;
}

template <typename T>
std::shared_ptr<T> Container<T>::LoadResource(const std::string& path)
{
	std::shared_ptr<T> newResource = std::make_shared<T>(path);
	if (const auto rValue = _resources.emplace(newResource->GetHash(), newResource); !rValue.second)
	{
		std::ostringstream os;
		os << "[Path]" << newResource->GetPath().c_str() << std::endl;
		os << "[Path]" << rValue.first->second->GetPath().c_str() << std::endl;
		os << "Have the same hash number please consider using another name";
		throw CONTAINER_EXCEPT(_name, os.str());
	}

	newResource->Load();
	return newResource;
}

template <typename T>
Container<T>* Container<T>::GetInstance()
{
	if (!_instance)
		_instance = new Container<T>();
	return _instance;
}

template <typename T>
void Container<T>::RemoveResources(const std::string& path)
{
	const unsigned long long eraseHash = StringConverter::HashStr(path);
	_resources.erase(eraseHash);
}

template <typename T>
void Container<T>::ClearUnusedResources()
{
	std::list<std::shared_ptr<T>> unusedResources;
	for (const auto& resource : _resources)
		if (resource.second.use_count() <= 1)
			if (resource.second->IsResourceUnused())
				unusedResources.push_back(resource.second);
	for (const auto& resource : unusedResources)
		_resources.erase(resource->GetHash());
}

template <typename T>
void Container<T>::ClearResources()
{
	_resources.clear();
}
#pragma endregion
