#pragma once
#include "json.hpp"

#include <string>
#include <unordered_map>

class Tag
{
	friend class Setting;
public:
	Tag(const std::string& tagStr);
	Tag(const char* tagStr);
	Tag(const Tag& tag);
	Tag(nlohmann::json& input);
	Tag() = default;
	Tag(Tag&&) = default;

	Tag& operator= (const Tag& tag) noexcept;

	[[nodiscard]] static std::string GetDelimeter() noexcept;

	[[nodiscard]] bool operator==(const Tag& tag) const noexcept;
	[[nodiscard]] bool operator!=(const Tag& tag) const noexcept;

	// Check if the tag contain the other tag
	[[nodiscard]] bool Contain(const Tag& tag) const noexcept;
	// Have at least one tag in common
	[[nodiscard]] bool Collide(const Tag& tag) const noexcept;
protected:
	static void Load(const std::string& tagStr);
	static void Load(nlohmann::json& input);
protected:
	std::string _tagStrings = "";
	// Bit mask of your tag
	unsigned _tagVar = 0;
private:
	inline static std::string _delimeter = "|";
	inline static std::unordered_map<std::string, unsigned> _decoder;
};

#define USING_TAG(varName, tagStr) static Tag varName(tagStr)