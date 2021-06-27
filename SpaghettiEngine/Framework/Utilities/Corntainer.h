#pragma once

#include <list>
#include <string>
#include <vector>
#include <functional>
#include "CollideEvent.h"

template<typename T>
class Corntainer
{
	using ListIterator = typename std::list<T>::iterator;
public:
	[[nodiscard]] std::list<T> GetAllItem();
	[[nodiscard]] unsigned		GetSize();

	virtual void AddItem(T item) = 0;

	virtual void RemoveAllItem();
	virtual void RemoveItem(T item);

	void IteratingWithLamda(const std::function<void(T)>& lamdaFun) const;
	void IteratingWithLamdaEvent(const std::function<void(T, CollideEvent&)>& lamdaFun, CollideEvent& e) const;
protected:

	// Start of loops
	ListIterator Begin() const;

	//  Took in an iterator and return whether it is at the end of the list or not
	bool Iterate(ListIterator& it) const;

	// Use this to erase to make sure that it work while iterating.
	ListIterator Erase(ListIterator) const;
	void Add(T item);

	std::list<T>& GetContainer() const;
protected:
	mutable bool _isIterating = false;
	mutable std::list<T> _container;
	mutable typename std::list<T>::iterator _currentIterator;
};

template<typename T>
inline std::list<T> Corntainer<T>::GetAllItem()
{
	return _container;
}

template<typename T>
inline unsigned Corntainer<T>::GetSize()
{
	return static_cast<unsigned>(_container.size());
}

template<typename T>
inline typename std::list<T>::iterator Corntainer<T>::Erase(ListIterator it) const
{
	if (_isIterating && it == _currentIterator)
	{
		_currentIterator = _container.erase(it);

		if (_currentIterator == _container.end())
			_isIterating = false;

		return _currentIterator;
	}
	else
		return _container.erase(it);
}

template<typename T>
inline void Corntainer<T>::Add(T item)
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
inline void Corntainer<T>::IteratingWithLamda(const std::function<void(T)>& lamdaFun) const
{
	ListIterator it = Begin();
	bool result = _container.empty();
	while (!result)
	{
		lamdaFun(*it);
		result = Iterate(it);
	}
}

template<typename T>
inline void Corntainer<T>::IteratingWithLamdaEvent(const std::function<void(T, CollideEvent&)>& lamdaFun, CollideEvent& e) const
{
	ListIterator it = Begin();
	bool result = _container.empty();
	while (!result)
	{
		lamdaFun(*it, e);
		result = Iterate(it);
	}
}

template<typename T>
inline typename std::list<T>::iterator Corntainer<T>::Begin() const
{
	_isIterating = true;
	_currentIterator = _container.begin();
	return _currentIterator;
}

template<typename T>
inline bool Corntainer<T>::Iterate(ListIterator& it) const
{
	if (!_isIterating)
		return true;

	++_currentIterator;
	it = _currentIterator;

	_isIterating = _currentIterator != _container.end();
	return !_isIterating;
}

template<typename T>
inline std::list<T>& Corntainer<T>::GetContainer() const
{
	return _container;
}
