#include "BoxCollider.h"
#include "GameTimer.h"

REGISTER_FINISH(BoxCollider);

BoxCollider::BoxCollider()
{
	name = TYPE_NAME(BoxCollider);
}

void BoxCollider::Start()
{

}

void BoxCollider::Update()
{

}

Vector3 BoxCollider::GetPosition()
{
	Vector3 pos;
	pos.x = owner->GetWorldTransform().x - width / 2;
	pos.y = owner->GetWorldTransform().y - height / 2;
	pos.z = owner->GetWorldTransform().z;
	return pos;
}

#define Width 0
#define Height 1

void BoxCollider::Load(const std::string* inputArg)
{
	try
	{
		TokenizedStr input = StringConverter::Tokenize(inputArg, ' ');
		try
		{
			width = std::stof(input[Width]);
			height = std::stof(input[Height]);
			Physic::GetInstance()->AddBoxCollider(this);
		}
		catch (CornException& e)
		{
			std::wostringstream os;
			os << L"Width " << input[Width].c_str() << L" Height " << input[Height].c_str()
				<< L" is out of bound";
			throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
		}
	}
	catch (CornException& e)
	{
		throw e;
	}
}
