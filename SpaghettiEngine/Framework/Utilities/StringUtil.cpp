#include "StringUtil.h"

void StrUtil::IterateSubStr(const std::function<void(const std::string&)>& lamdaFun, std::string str, const std::string& delimeter)
{
	size_t pos = 0;

	while ((pos = str.find(delimeter)) != std::string::npos)
	{
		lamdaFun(str.substr(0, pos));
		str.erase(0, pos + delimeter.size());
	}
	lamdaFun(str.substr(0, str.size()));
}

std::vector<std::string> StrUtil::Tokenize(const std::string& str, const std::string& delimeter)
{
	std::vector<std::string> tokens;
	IterateSubStr([&](const std::string& token) {
		tokens.emplace_back(token);
	}, str, delimeter);
	return tokens;
}