#include "BoxCollider2D.h"
#include "Physic.h"

REGISTER_FINISH(BoxCollider2D);

BoxCollider2D::BoxCollider2D(PScene owner) : Collider2DScriptBase(owner)
{
	_name = TYPE_NAME(BoxCollider2D);
	id = BoxCollider2DID;
}

Vector3 BoxCollider2D::GetSize()
{
	return size;
}

void BoxCollider2D::SetSize(Vector3 s)
{
	size = s;
}

void BoxCollider2D::Load(nlohmann::json& inputObject)
{
	std::string fieldTracker = "Start of the script";
	try
	{
		constexpr const char* Width = "Width";
		constexpr const char* Height = "Height";
		size.x = inputObject[Width] == nullptr ? 10 : inputObject[Width].get<float>();
		size.y = inputObject[Height] == nullptr ? 10 : inputObject[Height].get<float>();
	}
	catch (const CornException& e)
	{
		std::wostringstream os;
		os << L"Error field " << fieldTracker.c_str() << "\n\n" << e.What();
		std::wstring w = e.What();
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << "\n[Error field] " << fieldTracker.c_str() << "\n[Exception] " << e.what();
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	Collider2DScriptBase::Load(inputObject);
}


