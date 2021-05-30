#pragma once
#include <deque>
#include <list>

static struct ContainerUtil
{
	template<typename T>
	static inline bool Erase(std::deque<T>& container, const T& toErase)
	{
		for (auto it = container.begin(); it != container.end(); ++it)
			if (*it == toErase)
			{
				container.erase(it);
				return true;
			}
		return false;
	}

	template<typename T>
	static inline bool EmplaceBackUnique(std::deque<T>& container, const T& toEmplace)
	{
		for (auto it = container.begin(); it != container.end(); ++it)
			if (*it == toEmplace)
				return false;
		container.emplace_back(toEmplace);
		return true;
	}

	template<typename T>
	static inline bool Erase(std::list<T>& container, const T& toErase)
	{
		for (auto it = container.begin(); it != container.end(); ++it)
			if (*it == toErase)
			{
				container.erase(it);
				return true;
			}
		return false;
	}

	template<typename T>
	static inline bool EmplaceBackUnique(std::list<T>& container, const T& toEmplace)
	{
		for (auto it = container.begin(); it != container.end(); ++it)
			if (*it == toEmplace)
				return false;
		container.emplace_back(toEmplace);
		return true;
	}
};