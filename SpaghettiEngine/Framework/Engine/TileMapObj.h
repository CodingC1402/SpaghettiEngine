#include <GameObj.h>
#include <json.hpp>

using namespace nlohmann;
class TileMapObj : public GameObj
{
public:
	TileMapObj(const TileMapObj& obj);
	TileMapObj(const std::string& path, const PScene& ownerScene = nullptr);

};