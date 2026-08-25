#pragma once
#include "EngineInfo.h"
#include <filesystem>
#include "LogManager.h"
#include "../Manager/GameRuleManager.h"
#include "Asset/AssetSubManager.h"
#include "GameData.h"

class CGameDataManager :
    public CAssetSubManager
{
private:
	std::map<EGDataType, std::unordered_map<std::string, std::shared_ptr<CGameData>>> mMap;

public:
	virtual bool Init();

	template<typename T>
	bool LoadDataFile(const std::string& Name, EGDataType Type, const TCHAR* FileName)
	{
		if (mMap[Type].find(Name) != mMap[Type].end())
			return true;

		std::wstring filePath = FileName;
		switch (Type)
		{
		case EGDataType::Run:
			filePath.insert(0, TEXT("Run\\"));
			break;
		case EGDataType::Chapter:
			filePath.insert(0, TEXT("Chapter\\"));
			break;
		case EGDataType::Room:
			filePath.insert(0, TEXT("Room\\"));
			break;
		case EGDataType::Player:
			filePath.insert(0, TEXT("Player\\"));
			break;
		case EGDataType::Item:
			filePath.insert(0, TEXT("Item\\"));
			break;
		case EGDataType::Anim:
			filePath.insert(0, TEXT("Anim\\"));
			break;
		}

		std::shared_ptr<T> original = std::make_shared<T>();
		std::shared_ptr<CGameData> data = std::dynamic_pointer_cast<CGameData>(original);
		if (!data->Read(filePath.c_str()))
		{
			LOG_DEBUG("파일 로드 실패: ", Name.c_str());
			return false;
		}
		mMap[Type].insert(std::make_pair(Name, data));
		return true;
	}

	template<typename T>
	std::weak_ptr<T> FindData(const std::string& Name, EGDataType Type)
	{
		if (mMap[Type].find(Name) == mMap[Type].end())
			return std::weak_ptr<T>();
		return std::dynamic_pointer_cast<T>(mMap[Type][Name]);
	}

	template<typename T>
	std::weak_ptr<T> PeekRandom(EGDataType Type)
	{
		if (mMap[Type].empty())
			return std::weak_ptr<T>();

		float r = CGameRuleManager::GetInst()->GenerateRandomF();
		r = r * 100;
		int loop = static_cast<int>(r) % mMap[Type].size();
		std::unordered_map<std::string, std::shared_ptr<CGameData>>::iterator iter = std::next(mMap[Type].begin(), loop);
		return std::dynamic_pointer_cast<T>(iter->second);
	}

private:
	bool LoadAll();

	template <typename T>
	bool LoadFromDirectory(const std::wstring& RelativePath, EGDataType Type)
	{
		using namespace std::filesystem;

		std::wstring path = CPathManager::FindPath("Data");
		path += RelativePath;

		if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
			return false;

		for (const directory_entry& entry : directory_iterator(path))
		{
			std::wstring path = entry.path();
			std::fstream file(path);
			if (!file.is_open())
				continue;

			size_t loc = path.rfind('\\') + 1;
			std::wstring tmp = path.substr(loc, path.size() - loc);
			std::string name(tmp.begin(), tmp.end());

			LoadDataFile<T>(name, Type, std::wstring(name.begin(), name.end()).c_str());
		}

		return true;
	}
};

