#pragma once

// This is a comprehensive list of all the script that come with the engine 
// for easy debugging and figure out what field is need for the script.
// 
// It's difference from ref fields and common fields which is located in loading json class
// this a bunch of fields that usually belong to just one class
//

namespace Fields
{
	class Animator // Inherit from Render2DScriptBase
	{
	public:
		static constexpr const char* GetAnimationTreeField() { return "AnimationTree"; }
	};

	class Camera // Inherit from ScriptBase
	{
	public:
		// Nothing
	};

	class Render2DScriptBase // Inherit from ScriptBase
	{
	public:
		static constexpr const char* GetDrawLayerField() { return "DrawLayer"; }
	};

	class SpriteRenderer // Inherit from Render2DScriptBase
	{
	public:
		// TThe texture id in unsigned long long
		static constexpr const char* GetPathField()		{ return "Texture"; }
		// The index of the sprite that belong to the texture
		static constexpr const char* GetIndexField()	{ return "Index"; }

		// Off set in pixel to X or Y
		static constexpr const char* GetOffSetYField()	{ return "OffSetY"; }
		static constexpr const char* GetOffSetXField()	{ return "OffSetX"; }

		// Scale in x or y axis
		static constexpr const char* GetScaleXField()	{ return "ScaleX"; }
		static constexpr const char* GetScaleYField()	{ return "ScaleY"; }
	};

	class TileMapRenderer // Inherit from Render2DScriptBase
	{
	public:
		// The number of tile in row or column
		static constexpr const char* GetWidthField()		{ return "Width"; }
		static constexpr const char* GetHeightField()		{ return "Height"; }

		// Pixel width of a single tile
		static constexpr const char* GetTileWidthField()	{ return "TileWidth"; }
		static constexpr const char* GetTileHeightField()	{ return "TileHeight"; }

		// The actual tile matrix
		static constexpr const char* GetDataField()			{ return "Data"; }
	};

	class Surface // Inherit from ScriptBase
	{
	public:
		static constexpr const char* GetVelocityField()		{ return "Velocity"; }
		static constexpr const char* GetDirectionField()	{ return "Direction"; }
		static constexpr const char* GetIgnoreTag()			{ return "IgnoreTag"; }
	};

	//==================================================={ Physics }========================================================//
	class PhysicScriptBase // Inherit from ScriptBase
	{
	};

	class Collider2DBase // Inherit from PhysicScriptBase
	{
	public:
		static constexpr const char* GetOffSetXField() { return "OffSetX"; }
		static constexpr const char* GetOffSetYField() { return "OffSetY"; }
		static constexpr const char* GetTriggerField() { return "Trigger"; }
	};

	class CircleCollider // Inherit from Collider2DBase
	{
	public:
		static constexpr const char* GetRadiusField() { return "Radius"; }
	};

	class Polygon2DCollider // Inherit from Collider2DBase
	{
	public:
		static constexpr const char* GetVertexesField() { return "Vertexes"; }
	};

	class RigidBody2D // Inherit from PhysicScriptBase
	{
	public:
		static constexpr const char* GetVelocityField()		{ return "Velocity"; }
		static constexpr const char* GetMassField()			{ return "Mass"; }
		static constexpr const char* GetGravityScaleField() { return "GravityScale"; }
		static constexpr const char* GetMaterialField()		{ return "Material"; }
	};
}