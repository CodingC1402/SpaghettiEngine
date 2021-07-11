#include "DebugRenderer.h"
#include "Polygon2DCollider.h"
#include "polypartition.h"
#include "ScriptField.h"
#include "Setting.h"

REGISTER_FINISH(Polygon2DCollider, Collider2DBase) {}

void Polygon2DCollider::OnFixedUpdate()
{
	if constexpr (Setting::IsDebugMode())
	{
		if (_shapes.empty())
		{
			return;
		}

		auto worldMatrix = GetWorldMatrix();
		worldMatrix._41 += _shapes[0u]->GetOffSetX();
		worldMatrix._42 += _shapes[0u]->GetOffSetY();

		for (auto& shape : _shapes)
		{
			DebugRenderer::DrawPolygon(std::dynamic_pointer_cast<Polygon2D>(shape)->GetVertexes(), worldMatrix);
		}
	}
}

void Polygon2DCollider::Load(nlohmann::json& input)
{
	using Fields::Polygon2DCollider;

	_shapes.reserve(input.size());
	auto polygonsJson = input[Polygon2DCollider::GetPolygonsField()];
	std::vector<Vector3> polyVertexes;
	SPolygon newPoly;
	for (auto& poly : polygonsJson)
	{
		newPoly = std::make_shared<Polygon2D>();
		for (unsigned ver = 0u; ver < poly.size(); ver += 2)
			polyVertexes.push_back(Vector3(poly[ver].get<float>(), poly[static_cast<long long>(ver) + 1].get<float>(), 0.0f));
		newPoly->SetVertexes(polyVertexes);
		_shapes.push_back(newPoly);

		polyVertexes.clear();
	}

	Collider2DBase::Load(input);
}
