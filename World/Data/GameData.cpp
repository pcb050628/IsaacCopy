#include "GameData.h"

CGameData::CGameData(EGDataType Type)
	:mGDType(Type)
{
}

CGameData::~CGameData()
{
}

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

bool CGameData::WriteData(const TCHAR* FileName)
{
	StartWrite();
	Write(writer);
	if (!EndWrite())
		return false;

	const TCHAR* BasePath = CPathManager::FindPath("Data");

	TCHAR FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, BasePath);
	lstrcat(FullPath, FileName);

	std::ofstream outFile(FullPath, std::ios::out);
	if (!outFile.is_open())
		return false;

	outFile << buffer.GetString();

	outFile.close();
	return true;
}