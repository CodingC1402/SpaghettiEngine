#include "LineRenderBase.h"
#include "SMath.h"
#include "LoadingJson.h"
#include "Graphics.h"
#include "SMath.h"
#include "Camera.h"
#include "Vector3.h"

REGISTER_FINISH(LineRendererBase);

LineRendererBase::LineRendererBase(PScene owner, bool isDisabled) : ScriptBase(owner, isDisabled)
{
	_offSetMatrix = Matrix4::GetDiagonalMatrix();
}

void LineRendererBase::Draw(PCamera camera)
{
	auto matrix = camera->GetMatrixWithoutScaleY(GetGameObject()->GetTransform().GetWorldMatrix());
	auto transformedVectexes = _vertexes;
	for (auto& vectex : transformedVectexes)
		vectex = vectex * _offSetMatrix * matrix;
	Graphics::Draw2DPolygon(transformedVectexes, _color, _width);
}

void LineRendererBase::SetVertexes(const std::vector<Vector3>& vertextes)
{
	_vertexes = vertextes;
}

void LineRendererBase::SetCircle(float radius, const Vector3& center)
{
	if (radius <= 0)
		return;

	unsigned numberOfVertexes = static_cast<unsigned>(SMath::Lerp(_minVertexesForCircle, _vertextesForCircle, radius / _radiusToMaxVertexesNumber));
	if (numberOfVertexes == 0)
		return;
	_vertexes = std::vector<Vector3>(numberOfVertexes);
	_vertexes[0] = center;
	_vertexes[0].y += radius;

	Matrix4 rotationZ = SMath::GetZAxisRotateMatrix(360 / numberOfVertexes);
	for (int i = 1; i < numberOfVertexes; i++)
	{
		_vertexes[i] = _vertexes[static_cast<long long>(i) - 1] * rotationZ;
	}
}

void LineRendererBase::SetBox(const float width, const float height)
{
	_vertexes.clear();
	_vertexes.push_back(Vector3(-width / 2, -height / 2, 0));
	_vertexes.push_back(Vector3(width / 2, -height / 2, 0));
	_vertexes.push_back(Vector3(width / 2, height / 2, 0));
	_vertexes.push_back(Vector3(-width / 2, height / 2, 0));
}

void LineRendererBase::SetColor(Color color)
{
	_color = color;
}

void LineRendererBase::SetWidth(const float width)
{
	_width = width;
}

void LineRendererBase::SetOffSetX(const float x)
{
	_offSetMatrix._41 = x;
}

void LineRendererBase::SetOffSetY(const float y)
{
	_offSetMatrix._42 = y;
}

void LineRendererBase::OnEnabled()
{
	Graphics::AddLineRender(this);
}

void LineRendererBase::OnDisabled()
{
	Graphics::RemoveLineRender(this);
}

void LineRendererBase::Load(nlohmann::json& inputObject)
{
	constexpr auto vertexesField = "Vertexes";
	constexpr auto colorField = "Color";
	constexpr auto widthField = "Width";
	constexpr auto heightField = "Height";
	constexpr auto radiusField = "Radius";
	constexpr auto centerField = "Center";
	constexpr auto lineWidthField = "LineWidth";
	constexpr auto offSetXField = "OffSetX";
	constexpr auto offSetYField = "OffSetY";

	std::string type = inputObject[LoadingJson::Field::typeField].get<std::string>();
	if (type == "Polygon")
	{
		_vertexes = std::vector<Vector3>(inputObject[vertexesField].size());
		for (int i = 0; i < _vertexes.size(); i++)
		{
			auto vertexJson = inputObject[vertexesField][i];
			_vertexes[i].x = vertexJson[0];
			_vertexes[i].y = vertexJson[1];
			_vertexes[i].z = vertexJson[2];
		}
	}
	else if (type == "Rectangle")
	{
		SetBox(inputObject[widthField], inputObject[heightField]);
	}
	else
	{
		Vector3 center;
		if (inputObject[centerField] != nullptr)
		{
			center.x = inputObject[centerField][0];
			center.y = inputObject[centerField][1];
			center.z = inputObject[centerField][2];
		}

		SetCircle(inputObject[radiusField], center);
	}

	if (inputObject[colorField] != nullptr)
		_color = XRGB(inputObject[colorField][0], inputObject[colorField][1], inputObject[colorField][2]);

	if (inputObject[lineWidthField] != nullptr)
		_width = inputObject[lineWidthField].get<float>();

	if (inputObject[offSetXField] != nullptr)
		SetOffSetX(inputObject[offSetXField].get<float>());
	if (inputObject[offSetYField] != nullptr)
		SetOffSetY(inputObject[offSetYField].get<float>());
}

PScriptBase LineRendererBase::Clone() const
{
	auto clone = dynamic_cast<LineRendererBase*>(ScriptBase::Clone());

	clone->_color = _color;
	clone->_offSetMatrix = _offSetMatrix;
	clone->_vertexes = _vertexes;
	clone->_width = _width;

	return clone;
}
