#pragma  once
#include "ScriptBase.h"
#include "CornDirectX.h"
#include "Camera.h"

class Render2DScriptBase : public ScriptBase
{
public:
	virtual int GetDrawLayer();
	
	virtual void OnEnabled() override;
	virtual void OnDisabled() override;
	virtual void Draw(PCamera camera) {}
	virtual void Load(nlohmann::json& inputObject) override;

	PScriptBase Clone() const override;
protected:
	// Max is 32 layer;
	int _drawLayer = 0; 
private:
	REGISTER_START(Render2DScriptBase);
};