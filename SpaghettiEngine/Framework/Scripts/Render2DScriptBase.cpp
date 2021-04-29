#include "Render2DScriptBase.h"
#include "Graphics.h"

REGISTER_FINISH(Render2DScriptBase);

Render2DScriptBase::Render2DScriptBase()
{
	name = TYPE_NAME(Render2DScriptBase);
}

int Render2DScriptBase::GetDrawLayer()
{
	return _drawLayer;
}

void Render2DScriptBase::OnEnabled()
{
	Graphics::AddRender2D(this);
}

void Render2DScriptBase::OnDisabled()
{
	Graphics::RemoveRender2D(this);
}

void Render2DScriptBase::Load(nlohmann::json& inputObject)
{

	static constexpr const char* DrawLayer = "DrawLayer";
	try
	{
		_drawLayer = inputObject[DrawLayer] == nullptr ? 0 : inputObject[DrawLayer].get<int>();
		if (_drawLayer < 0 || _drawLayer > 31)
			throw SCRIPT_FORMAT_EXCEPT(this, std::string("\n[Error Field] ") + DrawLayer 
				+ "\n[Exception] DrawLayer can only be from 0 to 31");
	}
	catch (const CornException& e)
	{
		throw;
	}
	catch (const std::exception& e)
	{
		throw SCRIPT_FORMAT_EXCEPT(this, std::string("\n[Error Field] ") + DrawLayer + "\n[Exception] " + e.what());
	}

	ScriptBase::Load(inputObject);
}

