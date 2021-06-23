#pragma once

#include <list>
#include <string>
#include <vector>

template<typename T>
class Corntainer
{
public:
	T& operator[](unsigned index);
	[[nodiscard]] std::list<T> GetAllItem();
	[[nodiscard]] unsigned		GetSize();

	virtual void AddItem(T item);

	virtual void RemoveAllItem();
	virtual void RemoveItem(T item);
protected:
	std::list<T>& GetContainer() const;
protected:
	mutable std::list<T> _container;
};

template<typename T>
inline T& Corntainer<T>::operator[](unsigned index)
{
	return _container[index];
}

template<typename T>
inline std::list<T> Corntainer<T>::GetAllItem()
{
	return _container;
}

template<typename T>
inline unsigned Corntainer<T>::GetSize()
{
	return _container.size();
}

template<typename T>
inline void Corntainer<T>::AddItem(T item)
{
	_container.push_back(item);
}

template<typename T>
inline void Corntainer<T>::RemoveAllItem()
{
	_container.clear();
}

template<typename T>
inline void Corntainer<T>::RemoveItem(T item)
{
	for (auto it = _container.begin(); it != _container.end(); ++it)
		if ((*it) == item)
		{
			_container.erase(it);
			return;
		}
}

template<typename T>
inline std::list<T>& Corntainer<T>::GetContainer() const
{
	return _container;
}
