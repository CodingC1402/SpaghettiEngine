#pragma once

#define STATIC_CONSTRUCTOR(code) struct STATICCONSTRUCTOR {STATICCONSTRUCTOR(){code}}; static inline STATICCONSTRUCTOR _STATICCONSTRUCTOR;
namespace BTField
{
	constexpr auto fieldField = "Field";
	constexpr auto typeField = "Type";
	constexpr auto valueField = "Value";
	constexpr auto nameField = "Name";
	constexpr auto inputField = "Input";
	constexpr auto childrenField = "Children";
	constexpr auto blackBoardField = "BlackBoard";
	constexpr auto durationField = "Duration";
	constexpr auto oncePerTickField = "OncePerTick";
	constexpr auto loopField = "Loop";
}