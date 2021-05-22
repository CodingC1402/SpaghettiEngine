#include "TileMapCollider2D.h"
#include "BoxCollider2D.h"
#include "Physic.h"
#include "PolygonMath.h"
#include <fstream>

using namespace std;

REGISTER_FINISH(TileMapCollider2D);

TileMapCollider2D::TileMapCollider2D(PScene owner) : ScriptBase(owner)
{
	_name = TYPE_NAME(TileMapCollider2D);
}

void TileMapCollider2D::Load(nlohmann::json& input)
{
	std::string tileMapFilePath;

	constexpr const char* TileMapPath = "TileMapPath";
	constexpr const char* Points = "Points";
	constexpr const char* Colliders = "Colliders";

	try
	{
		tileMapFilePath = input[TileMapPath].get<std::string>();
	}
	catch (const std::exception&)
	{
		std::wostringstream os;
		os << "\n[Error field] " << TileMapPath;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	ifstream file(tileMapFilePath);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't exist" << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		VectorPointF base;

		for (auto index : jsonFile[Points])
		{
			base.push_back(Plane2D::PointF(index[0].get<float>(), index[1].get<float>()));
		}

		for (auto index : jsonFile[Colliders])
		{
			VectorPointF temp, result;
			for (int i : index)
			{
				temp.push_back(base[i]);
			}

			Triangulate::Process(temp, result);

			int tcount = result.size() / 3;

			for (int i = 0; i < tcount; i++)
			{
				Plane2D::PointF p1 = result[i * 3 + 0];
				Plane2D::PointF p2 = result[i * 3 + 1];
				Plane2D::PointF p3 = result[i * 3 + 2];
				//p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY(), p3.GetX(), p3.GetY()
			}

			temp.clear();
			result.clear();
		}

		base.clear();
		
		file.close();
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't have the right format" << std::endl;
		os << "[Exception] " << e.what() << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}
	ScriptBase::Load(input);
}
