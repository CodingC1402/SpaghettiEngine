#include "TileMapCollider2D.h"
#include "BoxCollider2D.h"
#include "Physic.h"
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

		if (jsonFile["ColliderType"].get<string>() == "BoxCollider2D")
		{
			auto newBoxCollider = new BoxCollider2D(nullptr);
			newBoxCollider->Load(jsonFile["Inputs"]);
			//Physic::GetInstance()->AddMapCollider(newBoxCollider);
		}
		
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
