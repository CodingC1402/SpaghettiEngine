#pragma once
#include <string>

namespace CLib
{
	[[nodiscard]]std::string GetPath(const std::string& path);
	[[nodiscard]]std::string GetFileName(const std::string& path);
	[[nodiscard]]std::string GetFileNameWithoutExtension(const std::string& path);
	[[nodiscard]]std::string GetExtension(const std::string& path);
	[[nodiscard]]std::string CombinePath(const std::string& path1, const std::string& path2);
	[[nodiscard]]std::string ConvertPath(const std::string& jsonFilePath, const std::string& propertyPath);
}