#pragma once
#include "EngineInfo.h"

class CGameObject;

class CGameClassContainer
{
	Singleton(CGameClassContainer)
private:
	std::map<int, std::function<std::weak_ptr<CGameObject>(const FVector2&)>> mMap;

public:
	template<typename T>
	const bool RegisterGameClass(const int ID, std::weak_ptr<CGameObject>(* Func)(const FVector2&))
	{
		if (mMap.contains(ID))
		{
			return false;
		}

		mMap[ID] = std::bind(Func, std::placeholders::_1);
		return true;
	}

	std::weak_ptr<CGameObject> Instantiate(const int ID, const FVector2& Coord)
	{
		if (!mMap.contains(ID))
			return std::weak_ptr<CGameObject>();
		return mMap[ID](Coord);
	}
};

