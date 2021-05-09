#include "BoxCollider2D.h"

REGISTER_FINISH(BoxCollider2D);

BoxCollider2D::BoxCollider2D(PScene owner) : Collider2DScriptBase(owner)
{
	_name = TYPE_NAME(BoxCollider2D);
}

Vector3 BoxCollider2D::GetSize()
{
	return size;
}

void BoxCollider2D::SetSize(Vector3 s)
{
	size = s;
}


void BoxCollider2D::Load(nlohmann::json& input)
{
	std::string fieldTracker = "Start of the script";
	try
	{
		constexpr const char* OffSetX = "OffSetX";
		constexpr const char* OffSetY = "OffSetY";
		constexpr const char* Width = "Width";
		constexpr const char* Height = "Height";
		constexpr const char* IsTrigger = "IsTrigger";
		
		offSet.x = input[OffSetX];
		offSet.y = input[OffSetY];

		size.x = input[Width];
		size.y = input[Height];

		isTrigger = input[IsTrigger];
	}
	catch (CornException& e)
	{
		throw;
	}
	catch (std::exception& e)
	{
		std::wostringstream os;
		os << L"Width " << input[Width] << L" Height " << input[Height]
			<< L" is out of bound";
		os << std::endl;
		os << e.what();
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}
}
