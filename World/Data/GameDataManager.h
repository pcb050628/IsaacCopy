#pragma once
#include "Asset/AssetSubManager.h"
#include "GameData.h"

class CGameDataManager :
    public CAssetSubManager
{
protected:
	std::unordered_map<std::string, std::shared_ptr<CGameData>> mMap;

public:
	virtual bool Init();

	template<typename T>
	bool LoadDataFile(const std::string& Name, const TCHAR* FileName)
	{
		if (mMap.find(Name) != mMap.end())
			return true;

		std::shared_ptr<T> original = std::make_shared<T>();
		std::shared_ptr<CGameData> data = std::dynamic_pointer_cast<CGameData>(original);
		if (!data->Read(Name))
			return false;
		mMap.insert(std::make_pair(Name, data));
		return true;
	}

	std::weak_ptr<CGameData> FindData(const std::string& Name);
};

