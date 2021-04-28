#include "Path.h"

using std::string;

std::string CLib::GetPath(const std::string& path)
{
	size_t index = path.size() - 1;
	while (index > 0)
	{
		if (path[index] == '/')
			return path.substr(0, index);
		index--;
	}
	return string();
}

std::string CLib::GetFileName(const std::string& path)
{
	size_t index = path.size() - 1;
	int count = 0;
	while (index > 0)
	{
		if (path[index] == '/')
		{
			if (index == path.size() - 1)
				break;
			else
				return path.substr(index + 1, count);
		}
		index--;
		count++;
	}
	return string();
}

std::string CLib::GetExtension(const std::string& path)
{
	size_t index = path.size() - 1;
	int count = 0;
	while (index > 0)
	{
		if (path[index] == '/')
			return string();
		if (path[index] == '.')
		{
			if (index == path.size() - 1)
				break;
			else
				return path.substr(index + 1, count);
		}
		index--;
		count++;
	}
	return string();
}

std::string CLib::CombinePath(const std::string& path1, const std::string& path2)
{
	if (path2.empty())
		return path1;
	if (path1.empty())
		return path2;
	
	string rPath = path1;
	if (rPath.back() == '/')
		rPath.pop_back();
	if (path2[0] == '/')
		rPath += path2;
	else
		rPath += '/' + path2;

	return rPath;
}
