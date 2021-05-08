#pragma once
#include <mutex>

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
	ThreadPtr(std::recursive_mutex* pad);
	ThreadPtr(const ThreadPtr& t);
	ThreadPtr& operator= (const ThreadPtr& t);
	Access operator->();

	Access GetAccess();
	T* GetPtr();
protected:
	void Lock();
	void Unlock();
protected:
	T* _rawPtr;
	std::recursive_mutex* _pad;
};

template<typename T>
inline ThreadPtr<T>::ThreadPtr(std::recursive_mutex* pad)
{
	_pad = pad;
}

template<typename T>
inline ThreadPtr<T>::ThreadPtr(const ThreadPtr& t)
{
	_rawPtr = t._rawPtr;
	_pad = t._pad;
}

template<typename T>
inline ThreadPtr<T>& ThreadPtr<T>::operator=(const ThreadPtr& t)
{
	_rawPtr = t._rawPtr;
	_pad = t._pad;
}

template<typename T>
inline ThreadPtr<T>::Access ThreadPtr<T>::operator->()
{
	return Access(this);
}

template<typename T>
inline ThreadPtr<T>::Access ThreadPtr<T>::GetAccess()
{
	return Access(this);
}

template<typename T>
inline T* ThreadPtr<T>::GetPtr()
{
	return _rawPtr;
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
	_owner->Lock();
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
	_owner = owner;
}
