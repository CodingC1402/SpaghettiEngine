#include "TriangleCollider2D.h"
#include "Physic.h"

REGISTER_FINISH(TriangleCollider2D);

TriangleCollider2D::TriangleCollider2D(PScene owner) : Collider2DScriptBase(owner)
{
	_name = TYPE_NAME(TriangleCollider2D);
	id = TriangleCollider2DID;
}

void TriangleCollider2D::Set(Vector3 al, Vector3 bt, Vector3 gm)
{
	A = al;
	B = bt;
	C = gm;
}

Vector3 TriangleCollider2D::GetA()
{
	return A;
}

Vector3 TriangleCollider2D::GetB()
{
	return B;
}

Vector3 TriangleCollider2D::GetC()
{
	return C;
}

void TriangleCollider2D::Create(Vector3 al, Vector3 bt, Vector3 gm)
{
	A = al;
	B = bt;
	C = gm;

	Physic::GetInstance()->AddMapCollider(this);
}

void TriangleCollider2D::Load(nlohmann::json& inputObject)
{
	std::string fieldTracker = "Start of the script";
	try
	{
		constexpr const char* Points = "Points";
		
		A = Vector3(inputObject[Points][0][0].get<float>(), inputObject[Points][0][1].get<float>(), inputObject[Points][0][2].get<float>());
		B = Vector3(inputObject[Points][1][0].get<float>(), inputObject[Points][1][1].get<float>(), inputObject[Points][1][2].get<float>());
		C = Vector3(inputObject[Points][2][0].get<float>(), inputObject[Points][2][1].get<float>(), inputObject[Points][2][2].get<float>());
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
