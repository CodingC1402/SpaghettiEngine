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

	// Load vertex
	std::vector<Vector3> loadedVertex(input[Polygon2DCollider::GetVertexesField()].size());

	for (unsigned i = 0u; i < loadedVertex.size(); i++)
	{
		loadedVertex[i].x = input[Polygon2DCollider::GetVertexesField()][i][0u];
		loadedVertex[i].y = input[Polygon2DCollider::GetVertexesField()][i][1u];
		loadedVertex[i].z = 0.0f;
	}
	
	TPPLPoly originalPoly;
	originalPoly.Init(static_cast<long>(loadedVertex.size()));
	for (unsigned i = 0u; i < loadedVertex.size(); i++)
	{
		originalPoly[i].x = loadedVertex[i].x;
		originalPoly[i].y = loadedVertex[i].y;
	}
	originalPoly.SetOrientation(TPPLOrientation::TPPL_ORIENTATION_CCW);
	originalPoly.SetHole(false);
	TPPLPolyList partitionList;
	TPPLPartition partition;

	partition.ConvexPartition_HM(&originalPoly, &partitionList); // Actual partitioning

	// In case of polygon being a concave
	unsigned polyParNum = static_cast<unsigned>(partitionList.size());
	_shapes.reserve(polyParNum);

	SPolygon newPoly;
	std::vector<Vector3> polyVertexes;
	for (auto it = partitionList.begin(); it != partitionList.end(); ++it)
	{
		newPoly = std::make_shared<Polygon2D>();
		for (int i = 0u; i < it->GetNumPoints(); i++)
			polyVertexes.push_back(Vector3(static_cast<float>((*it)[i].x), static_cast<float>((*it)[i].y), 0.0f));
		newPoly->SetVertexes(polyVertexes);

		_shapes.push_back(newPoly);

		polyVertexes.clear();
	}

	Collider2DBase::Load(input);
}
