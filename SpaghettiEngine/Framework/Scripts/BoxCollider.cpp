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
