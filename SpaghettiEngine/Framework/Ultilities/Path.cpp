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

std::string CLib::GetFileNameWithoutExtension(const std::string& path)
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
			{
				std::string name = path.substr(index + 1, count);
				if (name.find_first_of('.') == string::npos)
					return name;
				while (name.back() != '.')
					name.pop_back();
				name.pop_back();
				return name;
			}
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

std::string CLib::ConvertPath(const std::string& jsonFilePath, const std::string& propertyPath)
{
	if (propertyPath.empty())
		return std::string();
	if (propertyPath[0] != '{')
		return propertyPath;
	for (size_t i = 0; i < propertyPath.size(); i++)
	{
		if (propertyPath[i] == '}')
		{
			const std::string pathExtension = propertyPath.substr(1, i - 1);
			const std::string propertyPathWithoutEx = propertyPath.substr(i + 2);
			std::string rPath = GetPath(jsonFilePath);
			if (pathExtension == "ChildPath")
				rPath +=  '/' + GetFileNameWithoutExtension(jsonFilePath);
			rPath = CombinePath(rPath, propertyPathWithoutEx);
			return rPath;
		}
	}
	return propertyPath;
}
