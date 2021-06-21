#include "Polygon2DCollider.h"
#include "LineRenderBase.h"
#include "Setting.h"
#include "polypartition.h"

REGISTER_FINISH(Polygon2DCollider);

Polygon2DCollider::Polygon2DCollider(PScene owner, bool isDisabled) : Collider2DBase(owner , isDisabled)
{}

void Polygon2DCollider::Load(nlohmann::json& input)
{
	// Load vertex
	std::vector<Vector3> loadedVertex(input[_vertexesField].size());
	Collider2DBase::Load(input);
	for (unsigned i = 0; i < loadedVertex.size(); i++)
	{
		loadedVertex[i].x = input[_vertexesField][i][0];
		loadedVertex[i].y = input[_vertexesField][i][1];
		loadedVertex[i].z = 0;
	}
	
	TPPLPoly originalPoly;
	originalPoly.Init(static_cast<long>(loadedVertex.size()));
	for (int i = 0; i < loadedVertex.size(); i++)
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
	_shapePolygon.reserve(polyParNum);
	_shapes.reserve(polyParNum);

	SPolygon newPoly;
	std::vector<Vector3> polyVertexes;
	for (auto it = partitionList.begin(); it != partitionList.end(); ++it)
	{
		newPoly = std::make_shared<Polygon2D>();
		for (int i = 0; i < it->GetNumPoints(); i++)
			polyVertexes.push_back(Vector3(static_cast<float>((*it)[i].x), static_cast<float>((*it)[i].y), 0.0f));
		newPoly->SetVertexes(polyVertexes);

		_shapePolygon.push_back(newPoly);
		_shapes.push_back(newPoly);

		if constexpr (Setting::IsDebugMode())
		{
			_lineRenderer.emplace_back(new LineRendererBase(_owner));
			_lineRenderer.back()->SetVertexes(polyVertexes);
		}
		polyVertexes.clear();
	}

	if constexpr (Setting::IsDebugMode())
		SetLineRendererOwner();

	Collider2DBase::Load(input);
}
