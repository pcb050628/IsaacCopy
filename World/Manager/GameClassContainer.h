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
	std::unordered_map<int, std::function<std::weak_ptr<CGameObject>(const FVector2&, bool, int)>> mWorldObjMap;
	std::unordered_map<int, std::function<std::shared_ptr<CGameObject>()>> mDefMap;

public:
	template<typename T>
	const bool RegisterGameClass(const int ID, std::weak_ptr<CGameObject>(* Func)(const FVector2&, bool, int))
	{
		if (mWorldObjMap.contains(ID))
		{
			return false;
		}

		mWorldObjMap[ID] = std::bind(Func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		return true;
	}
	template<typename T>
	const bool RegisterGameClass(const int ID, std::shared_ptr<CGameObject>(*Func)())
	{
		if (mDefMap.contains(ID))
		{
			return false;
		}

		mDefMap[ID] = std::bind(Func);
		return true;
	}

	std::weak_ptr<CGameObject> Instantiate(const int ID, const FVector2& Coord, bool OnFocus = true, const int ChapterLevel = 0)
	{
		if (!mWorldObjMap.contains(ID))
			return std::weak_ptr<CGameObject>();
		return mWorldObjMap[ID](Coord, OnFocus, ChapterLevel);
	}
	std::shared_ptr<CGameObject> InstantiateDef(const int ID)
	{
		if (!mDefMap.contains(ID))
			return std::shared_ptr<CGameObject>();
		return mDefMap[ID]();
	}

	void MakeWithData(class CGameData* GData);

private:
	void MakeChapter(class CChapterGData* GData, bool IsFocused = false);
};


