#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>

class Resource
{
public:
	Resource(const Resource&) = delete;
	Resource(const Resource&&) = delete;
	Resource& operator= (const Resource&) = delete;
	Resource& operator= (const Resource&&) = delete;
protected:
	Resource(const std::string& path);
	virtual ~Resource() = default;
	
	virtual void Load() = 0;
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

	static Container* GetInstance();
protected:
	Container() = default;
	virtual ~Container() = default;
protected:
	std::map<unsigned long long, T*> _resources;

	static Container* _instance;
};

template <typename T>
Container<T>* Container<T>::GetInstance()
{
	if (!_instance)
		_instance = new Container<T>();
	return _instance;
}
