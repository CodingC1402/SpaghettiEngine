#pragma once
#include <string>

namespace CLib
{
	std::string GetPath(const std::string& path);
	std::string GetFileName(const std::string& path);
	std::string GetExtension(const std::string& path);
	std::string CombinePath(const std::string& path1, const std::string& path2);
}