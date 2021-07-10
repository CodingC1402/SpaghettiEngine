#pragma once
#include <functional>
#include <vector>

namespace StrUtil
{
	void IterateSubStr(const std::function<void(const std::string&)>& lamdaFun, std::string str, const std::string& delimeter);
	std::vector<std::string> Tokenize(const std::string& str, const std::string& delimeter);
}