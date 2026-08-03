#pragma once
#include "EngineInfo.h"

class CGameObject;

//1-방 (예시: 10, 11, 141, 111, 101, 15 등
//2-적 (예시: 20, 21, 22, 211, 213, 24 등
//6-픽업
//7-아이템
//9-보스
class CGameClassContainer
{
	Singleton(CGameClassContainer)
private:
	std::map<int, std::function<std::weak_ptr<CGameObject>(const FVector2&, int)>> mMap;

public:
	template<typename T>
	const bool RegisterGameClass(const int ID, std::weak_ptr<CGameObject>(* Func)(const FVector2&, int))
	{
		if (mMap.contains(ID))
		{
			return false;
		}

		mMap[ID] = std::bind(Func, std::placeholders::_1, std::placeholders::_2);
		return true;
	}

	std::weak_ptr<CGameObject> Instantiate(const int ID, const FVector2& Coord, const int ChapterLevel = 0)
	{
		if (!mMap.contains(ID))
			return std::weak_ptr<CGameObject>();
		return mMap[ID](Coord, ChapterLevel);
	}

	void MakeWithData(class CGameData* GData);

private:
	void MakeChapter(class CChapterGData* GData, bool IsFocused = false);
	void MakeRoom(class CRoomGData* GData);
	void MakePlayer(class CPlayerGData* GData);
};


