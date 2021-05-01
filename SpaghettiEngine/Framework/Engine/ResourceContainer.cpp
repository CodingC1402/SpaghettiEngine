#include "ResourceContainer.h"

Resource::ResourceException::ResourceException(int line, const char* file, const std::string& descrpition, const std::string& resourceType)
	: CornException(line, file),
	_description(descrpition),
	_resourceType(resourceType)
{}

const wchar_t* Resource::ResourceException::What() const noexcept
{
	return L"Σ(▼□▼メ) Resource loading exception";
}

const wchar_t* Resource::ResourceException::GetType() const noexcept
{
	std::wostringstream os;
	os << GetOriginString() << std::endl;
	os << "[Resource type] " << _resourceType.c_str() << std::endl;
	os << _description.c_str() << std::endl;
	
	whatBuffer = os.str();
	return whatBuffer.c_str();
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
