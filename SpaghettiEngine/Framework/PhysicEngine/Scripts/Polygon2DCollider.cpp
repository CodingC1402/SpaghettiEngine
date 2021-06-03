#include "Polygon2DCollider.h"
#include "LineRenderBase.h"
#include "Setting.h"

REGISTER_FINISH(Polygon2DCollider);

Polygon2DCollider::Polygon2DCollider(PScene owner, bool isDisabled) : Collider2DBase(owner , isDisabled)
{
	_shapePolygon = std::make_shared<Polygon2D>();
	_shape = _shapePolygon;
	_name = TYPE_NAME(Polygon2DCollider);
}

void Polygon2DCollider::Load(nlohmann::json& input)
{
	std::vector<Vector3> loadedVertex(input[_vertexesField].size());
	for (unsigned i = 0; i < loadedVertex.size(); i++)
	{
		loadedVertex[i].x = input[_vertexesField][i][0];
		loadedVertex[i].y = input[_vertexesField][i][1];
		loadedVertex[i].z = input[_vertexesField][i][2];
	}
	_shapePolygon->SetVertexes(loadedVertex);

	if constexpr (Setting::IsDebugMode())
		_lineRenderer->SetVertexes(loadedVertex);
}
