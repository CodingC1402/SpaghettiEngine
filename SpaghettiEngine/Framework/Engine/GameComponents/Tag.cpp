#include "Tag.h"
#include "StringUtil.h"

Tag::Tag(const std::string& tagStr)
{
	_tagStrings = tagStr;

	StrUtil::IterateSubStr([&](const std::string& token) {
		_tagVar |= _decoder[token];
	}, tagStr, _delimeter);
}

Tag::Tag(const char* tagStr)
{
	_tagStrings = tagStr;

	StrUtil::IterateSubStr([&](const std::string& token) {
		_tagVar |= _decoder[token];
	}, tagStr, _delimeter);
}

Tag::Tag(const Tag& tag)
{
	*this = tag;
}

Tag::Tag(nlohmann::json& input)
{
	std::string tagStr;
	for (auto& tag : input)
	{
		tagStr = tag.get<std::string>();
		_tagStrings += tagStr + _delimeter;
		_tagVar |= _decoder[tagStr];
	}
	_tagStrings.erase(_tagStrings.size() - _delimeter.size(), _delimeter.size());
}

Tag& Tag::operator=(const Tag& tag) noexcept
{
	_tagStrings = tag._tagStrings;
	_tagVar = tag._tagVar;
	return *this;
}

std::string Tag::GetDelimeter() noexcept
{
	return _delimeter;
}

Tag Tag::operator|(const Tag& tag) const noexcept
{
	Tag rValue;
	rValue._tagVar = _tagVar | tag._tagVar;
	return rValue;
}

Tag Tag::operator&(const Tag& tag) const noexcept
{
	Tag rValue;
	rValue._tagVar = _tagVar & tag._tagVar;
	return rValue;
}

bool Tag::operator==(const Tag& tag) const noexcept
{
	return _tagVar == tag._tagVar;
}

bool Tag::operator!=(const Tag& tag) const noexcept
{
	return _tagVar != tag._tagVar;
}

bool Tag::Contain(const Tag& tag) const noexcept
{
	return tag._tagVar == (_tagVar & tag._tagVar);
}

bool Tag::Collide(const Tag& tag) const noexcept
{
	return _tagVar & tag._tagVar;
}

void Tag::Load(const std::string& tagStr)
{
	unsigned offSet = 0;
	_decoder.clear();
	StrUtil::IterateSubStr([&](const std::string& token) {
		_decoder.emplace(token, 1u << offSet);
		offSet++;
	}, tagStr, _delimeter);
}

void Tag::Load(nlohmann::json& input)
{
	unsigned offSet = 0;
	_decoder.clear();
	for (auto& tag : input)
	{
		_decoder.emplace(tag.get<std::string>(), 1u << offSet);
		offSet++;
	}
}
