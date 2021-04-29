#include "ResourceContainer.h"

Resource::Resource(const std::string& path)
	:
	_path(path),
	_hash(StringConverter::HashStr(path))
{}

std::string Resource::GetPath() const
{
	return _path;
}

unsigned long long Resource::GetHash() const
{
	return _hash;
}

bool Resource::IsResourceUnused() const
{
	return true;
}

ContainerException::ContainerException(int line, const char* file, const std::string& containerName,
                                       const std::string& description)
	: CornException(line, file),
	_containerName(containerName),
	_description(description)
{}


const wchar_t* ContainerException::What() const noexcept
{
	std::wostringstream os;
	os << GetOriginString() << std::endl;
	os << "[Container type] " << _containerName.c_str() << std::endl;
	os << _description.c_str() << std::endl;

	whatBuffer = os.str();
	return whatBuffer.c_str();
}
const wchar_t* ContainerException::GetType() const noexcept
{
	return L"((╬◣﹏◢)) Container exception";
}
