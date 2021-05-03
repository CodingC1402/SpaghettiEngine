#include "BoxCollider.h"
#include "GraphicsMath.h"
#include "GameTimer.h"
#include "Physic.h"
#include "Setting.h"
REGISTER_FINISH(BoxCollider);

BoxCollider::BoxCollider()
{
	name = TYPE_NAME(BoxCollider);
}

void BoxCollider::Start()
{
	rigidbody = dynamic_cast<PRigidBody>(owner->GetScript("RigidBody"));
}


Vector3* BoxCollider::GetPosition() const
{
	Vector3* pos = new Vector3;
	pos->x = owner->GetWorldTransform().x - width / 2;
	pos->y = owner->GetWorldTransform().y - height / 2;
	pos->z = owner->GetWorldTransform().z;
	return pos;
}

Vector3 BoxCollider::GetVelocity()
{
	if (rigidbody == nullptr)
		return Vector3(0, 0, 0);

	return rigidbody->GetVelocity();
}

void BoxCollider::DrawBox(RenderDevice render, PCamera camera, Color color)
{
	Matrix* transform = new Matrix;
	*transform = camera->GetMatrix(GetWorldMatrix());

	if (Setting::IsWorldPointPixelPerfect())
	{
		transform->_41 = std::round(transform->_41);
		transform->_42 = std::round(transform->_42);
	}

	Vector3* transformPosition = new Vector3;
	GraphicsMath::TransformVector3(transformPosition, this->GetPosition(), transform);
	
	D3DRECT rect = { transformPosition->x,
		transformPosition->y,
		transformPosition->x + this->width,
		transformPosition->y + this->height };
	
	render->Clear(1, &rect, D3DCLEAR_TARGET, color, 1.0f, NULL);
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
		catch (std::exception& e)
		{
			std::wostringstream os;
			os << L"Width " << input[Width].c_str() << L" Height " << input[Height].c_str()
				<< L" is out of bound";
			os << std::endl;
			os << e.what();
			throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
		}
	}
	catch (CornException& e)
	{
		throw e;
	}
}
