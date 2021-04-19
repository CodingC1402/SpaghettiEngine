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
	pos.x = owner->GetPosition()->x - width / 2;
	pos.y = owner->GetPosition()->y - height / 2;
	pos.z = owner->GetPosition()->z;
	return pos;
}

#define Width 0
#define Height 1

void BoxCollider::Load(const std::string* inputArg)
{
	try
	{
		TokenizedStr input = StringConverter::Tokenize(inputArg, ' ');
		width = std::stof(input[Width]);
		height = std::stof(input[Height]);
		Physic::GetInstance()->AddBoxCollider(this);
	}
	catch (CornException& e)
	{
		throw e;
	}
}
