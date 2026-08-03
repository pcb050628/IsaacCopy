#include "GameData.h"

bool CGameData::Load(const TCHAR* FileName, rapidjson::Document& Doc)
{
	const TCHAR* BasePath = CPathManager::FindPath("Data");

	TCHAR FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, BasePath);
	lstrcat(FullPath, FileName);

	std::fstream file(FullPath);
	if (!file.is_open())
		return false;

	std::string str;
	std::string tmp;
	while (std::getline(file, tmp))
	{
		str += tmp;
	}
	Doc.Parse(str.c_str());

	file.close();
	return true;
}

void CGameData::StartWrite()
{
	buffer.Clear();
	writer.Reset(buffer);
	writer.StartObject();
}
bool CGameData::EndWrite()
{
	writer.EndObject();
	if (!writer.IsComplete())
		return false;

	return true;
}
