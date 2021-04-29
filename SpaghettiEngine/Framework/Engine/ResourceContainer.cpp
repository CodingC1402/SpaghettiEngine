#include "ResourceContainer.h"
#include "StringConverter.h"

Resource::Resource(const std::string& path)
	:
	_path(path),
	_hash(StringConverter::HashStr(path))
{}
