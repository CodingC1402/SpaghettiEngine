#pragma once
#include <mutex>
#include <list>

template<typename T>
class ThreadList
{
public:
	class Access {
	public:
		std::list<T>* operator->() {
			return _ptr;
		}
		std::list<T>& operator[](const unsigned index) {
			return *_ptr;
		}

		~Access() {
			_ptr = nullptr;
			_lock->unlock();
		}
		Access(std::list<T>* ptr, std::recursive_mutex& lock) : _ptr(ptr), _lock(&lock) {
			_lock->lock();
		}
	protected:
		std::recursive_mutex* _lock;
		std::list<T>* _ptr;
	};
public:
	ThreadList() {
		_list = new std::list<T>();
	}
	~ThreadList() {
		delete _list;
		_list = nullptr;
	}
	Access operator->() {
		return Access(_list, _lock);
	}
	Access Use () {
		return Access(_list, _lock);
	}
protected:
	std::recursive_mutex _lock;
	std::list<T>* _list;
};

template<typename T>
class ThreadPtr
{
public:
	class Access
	{
	public:
		T* operator->();

		~Access();
		Access(ThreadPtr* owner);
	protected:
		ThreadPtr* _owner;
	};
public:
	ThreadPtr(T* rawPtr);
	ThreadPtr(const ThreadPtr& t);
	ThreadPtr& operator= (const ThreadPtr& t);
	ThreadPtr& operator= (T* t);
	Access operator->();

	Access GetAccess();
	T* GetPtr();

	~ThreadPtr();
protected:
	void Lock();
	void Unlock();
protected:
	T* _rawPtr;
	std::recursive_mutex *_pad;
	unsigned* _counter;
};

template<typename T>
inline ThreadPtr<T>::ThreadPtr(T* rawPtr)
{
	_rawPtr = rawPtr;
	_pad = new std::recursive_mutex();
	_counter = new unsigned();
	*_counter = 1;
}

template<typename T>
inline ThreadPtr<T>::ThreadPtr(const ThreadPtr& t)
{
	_rawPtr = t._rawPtr;
	_pad = t._pad;
	_counter = t._counter;
	*_counter = *_counter + 1;
}

template<typename T>
inline ThreadPtr<T>& ThreadPtr<T>::operator=(const ThreadPtr& t)
{
	_rawPtr = t._rawPtr;
	_pad = t._pad;
	_counter = t._counter;
	*_counter = *_counter + 1;
}

template<typename T>
inline ThreadPtr<T>& ThreadPtr<T>::operator=(T* t)
{
	_rawPtr = t;
	_pad = new std::recursive_mutex();
	_counter = new unsigned();
	*_counter = 1;
}

template<typename T>
inline typename ThreadPtr<T>::Access ThreadPtr<T>::operator->()
{
	return Access(this);
}

template<typename T>
inline typename ThreadPtr<T>::Access ThreadPtr<T>::GetAccess()
{
	return Access(this);
}

template<typename T>
inline T* ThreadPtr<T>::GetPtr()
{
	return _rawPtr;
}

template<typename T>
inline ThreadPtr<T>::~ThreadPtr()
{
	*_counter -= 1;
	if (*_counter == 0)
	{
		delete _rawPtr;
		delete _counter;
		delete _pad;
	}
}

template<typename T>
inline void ThreadPtr<T>::Lock()
{
	_pad.lock();
}

template<typename T>
inline void ThreadPtr<T>::Unlock()
{
	_pad.unlock();
}

template<typename T>
inline T* ThreadPtr<T>::Access::operator->()
{
	return _owner->_rawPtr;
}

template<typename T>
inline ThreadPtr<T>::Access::~Access()
{
	_owner->Unlock();
}

template<typename T>
inline ThreadPtr<T>::Access::Access(ThreadPtr* owner)
{
	_owner->Lock();
	_owner = owner;
}
