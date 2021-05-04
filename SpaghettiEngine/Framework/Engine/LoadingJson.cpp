#include "LoadingJson.h"
#include "Scene.h"
#include "CornException.h"

std::unordered_map<std::string, Scene::ComponentType> LoadingJson::ID::_typeDict {
	{"GameObject", Scene::ComponentType::gameObj},
	{"Script", Scene::ComponentType::script}
};

void LoadingJson::ID::ConvertIDInJson(nlohmann::json& out, Scene::ComponentType type)
{
	try
	{
		unsigned prefabID = 0;

		if (out[prefabIdField] != nullptr)
			prefabID = out[prefabIdField].get<unsigned>();
				
		out[idField] = ID::CreateTopLevelID(
			out[idField].get<unsigned>(),
			type,
			prefabID
		);

		if(out[inputsField][gameObjectsField] != nullptr)
			for (auto& child : out[inputsField][gameObjectsField])
				child = ID::CreateTopLevelID(
					child.get<unsigned>(),
					Scene::ComponentType::gameObj,
					prefabID
				);
		if(out[inputsField][scriptsField] != nullptr)
			for (auto& script : out[inputsField][scriptsField])
				script = ID::CreateTopLevelID(
					script.get<unsigned>(),
					Scene::ComponentType::script,
					prefabID
				);
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << L"[Exception] the json object is in a wrong format, exception throw when converting id\n";
		os << e.what() << std::endl;
		std::ostringstream os2;
		os2 << out;
		os << L"[Error json object] " << os2.str().c_str() << std::endl;
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}
}

Scene::ComponentType LoadingJson::ID::ConvertStrToType(const std::string& str)
{
	const auto rValue = _typeDict.find(str);
	if (rValue == _typeDict.end())
		return Scene::ComponentType::invalid;
	return rValue->second;
}
