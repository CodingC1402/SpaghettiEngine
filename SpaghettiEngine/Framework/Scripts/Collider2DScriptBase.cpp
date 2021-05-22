#include "Collider2DScriptBase.h"
#include "Physic.h"

REGISTER_FINISH(Collider2DScriptBase);

Collider2DScriptBase::Collider2DScriptBase(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(Collider2DScriptBase);
}

void Collider2DScriptBase::OnEnabled()
{
	Physic::GetInstance()->AddCollider(this);
}

void Collider2DScriptBase::OnDisabled()
{
	Physic::GetInstance()->RemoveCollider(this);
}

void Collider2DScriptBase::OnStart()
{
	body = dynamic_cast<PRigidBody2D>(_ownerObj->GetScript("RigidBody"));
}

void Collider2DScriptBase::OnUpdate()
{
	if (!_ownerObj)
		return;
	
	position = _ownerObj->GetWorldTransform();

	ScriptBase::OnUpdate();
}

Vector3 Collider2DScriptBase::GetPosition()
{
	if (!_ownerObj)
		return position;
	return _ownerObj->GetWorldTransform();
}

Vector3 Collider2DScriptBase::GetVelocity()
{
	if (!body)
		return Vector3(0, 0, 0);

	return body->GetVelocity();
}

long Collider2DScriptBase::GetID()
{
	return id;
}

void Collider2DScriptBase::SetTrigger(bool boolean)
{
	isTrigger = boolean;
}

void Collider2DScriptBase::SetOffSet(Vector3 offset)
{
	offSet = offset;
}

void Collider2DScriptBase::Load(nlohmann::json& inputObject)
{
	std::string fieldTracker = "Start of the script";
	try
	{
		constexpr const char* OffSetX = "OffSetX";
		constexpr const char* OffSetY = "OffSetY";
		constexpr const char* Trigger = "Trigger";
		constexpr const char* PosX = "PosX";
		constexpr const char* PosY = "PosY";
		offSet.x = inputObject[OffSetX] == nullptr ? 0 : inputObject[OffSetX].get<float>();
		offSet.y = inputObject[OffSetY] == nullptr ? 0 : inputObject[OffSetY].get<float>();
		isTrigger = inputObject[Trigger] == nullptr ? false : inputObject[Trigger].get<bool>();
		position.x = inputObject[PosX] == nullptr ? 0 : inputObject[PosX].get<float>();
		position.y = inputObject[PosY] == nullptr ? 0 : inputObject[PosY].get<float>();
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

	ScriptBase::Load(inputObject);
}
