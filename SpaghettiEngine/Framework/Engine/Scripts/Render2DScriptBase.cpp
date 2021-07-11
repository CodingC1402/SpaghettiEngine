#include "Render2DScriptBase.h"
#include "Graphics.h"
#include "ScriptField.h"

REGISTER_FINISH(Render2DScriptBase, ScriptBase) {}

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
	using Fields::Render2DScriptBase;

	try
	{
		_drawLayer = inputObject[Render2DScriptBase::GetDrawLayerField()] == nullptr ? 0 : inputObject[Render2DScriptBase::GetDrawLayerField()].get<int>();
		if (_drawLayer < 0 || _drawLayer > 31)
		{
			std::wostringstream os;
			os << "\n[Error Field] " << Render2DScriptBase::GetDrawLayerField() << "\n[Exception] DrawLayer can only be from 0 to 31";
			throw SCRIPT_FORMAT_EXCEPT(this, os.str());
		}
	}
	catch (const CornException&)
	{
		throw;
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << "\n[Error Field] " << Render2DScriptBase::GetDrawLayerField() << "\n[Exception] " << e.what();
	}

	ScriptBase::Load(inputObject);
}

PScriptBase Render2DScriptBase::Clone() const
{
	auto clone = dynamic_cast<Render2DScriptBase*>(ScriptBase::Clone());

	clone->_drawLayer = _drawLayer;

	return clone;
}

