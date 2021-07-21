#pragma once
#include "Plane2D.h"
#include "CornException.h"
#include "SMath.h"
#include <string>

// To disable draw quad tree just remove this.
// To enable draw quad tree use this
// #define DRAW_QUAD_TREE_GRID

class Setting
{
public:
	class AppSettingException : public CornDescriptionException
	{
	public:
		AppSettingException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	static Setting* GetInstance();
	static Plane2D::Size& GetResolution();
	static Plane2D::Size& GetHalfResolution();
	static const wchar_t* GetAppName();
	static bool IsResolutionPixelPerfect();
	static bool IsWorldPointPixelPerfect();
	static float GetFps();
	
	[[nodiscard]] static constexpr bool IsDebugMode();
	[[nodiscard]] static constexpr bool IsDrawQuadTree();
	[[nodiscard]] static constexpr bool IsDrawQuadTreeGrid();
protected:
	~Setting() = default;
	Setting();
	void Load();
protected:
	Plane2D::Size resolution;
	Plane2D::Size halfResolution;

	std::wstring name;

	bool isResolutionPixelPerfect = false;
	bool isWorldPointPixelPerfect = false;
	float fps;

	static Setting* __instance;
	
#ifdef _DEBUG
	static constexpr bool _isDebugMode = true;
#else
	static constexpr bool _isDebugMode = true;
#endif

#ifdef DRAW_QUAD_TREE
	static constexpr bool _isDrawQuadTree = true;
#else
	static constexpr bool _isDrawQuadTree = false;
#endif

#ifdef DRAW_QUAD_TREE_GRID
	static constexpr bool _isDrawQuadTreeGrid = true;
#else
	static constexpr bool _isDrawQuadTreeGrid = false;
#endif
};

constexpr bool Setting::IsDebugMode()
{
	return _isDebugMode;
}

constexpr bool Setting::IsDrawQuadTree()
{
	return _isDrawQuadTree;
}

constexpr bool Setting::IsDrawQuadTreeGrid()
{
	return _isDrawQuadTreeGrid;
}

#define APPSETTING_EXCEPT(description) Setting::AppSettingException(__LINE__,__FILE__,description)
