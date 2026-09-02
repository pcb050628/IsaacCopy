#pragma once
#include "EngineInfo.h"

#include "../Base/GameDefinition.h"

class CGameObject;
//class CGameDefinition;

//1-방 (예시: 10, 11, 141, 111, 101, 15 등
//2-적 (예시: 20, 21, 22, 211, 213, 24 등
//7-아이템
//8-픽업
//9-보스
class CGameClassContainer
{
	Singleton(CGameClassContainer);
private:
	std::unordered_map<int, std::function<std::weak_ptr<CGameObject>(const FVector2&, bool, int)>> mWorldObjMap;
	std::unordered_map<int, std::function<std::shared_ptr<CGameDefinition>(const std::weak_ptr<CGameClass>&)>> mDefMap;

	std::unordered_map<int, std::string> mNameMap;

public:
	const bool RegisterGameClass(const int ID, const std::string& Name, std::weak_ptr<CGameObject>(* Func)(const FVector2&, bool, int))
	{
		if (mWorldObjMap.contains(ID))
		{
			std::string error = "같은 아이디를 사용하는 객체가 있습니다. 아이디:" + std::to_string(ID);
			assert(false && error.c_str());
			return false;
		}

		mWorldObjMap[ID] = std::bind(Func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		mNameMap[ID] = Name;
		return true;
	}
	const bool RegisterGameClass(const int ID, const std::string& Name, std::shared_ptr<CGameDefinition>(*Func)(const std::weak_ptr<CGameClass>&))
	{
		if (mDefMap.contains(ID))
		{
			std::string error = "같은 아이디를 사용하는 객체가 있습니다. 아이디:" + std::to_string(ID);
			assert(false && error.c_str());
			return false;
		}

		mDefMap[ID] = std::bind(Func, std::placeholders::_1);
		mNameMap[ID] = Name;
		return true;
	}

	std::weak_ptr<CGameObject> Instantiate(const int ID, const FVector2& Coord, bool OnFocus = true, const int ChapterLevel = 0)
	{
		if (!mWorldObjMap.contains(ID))
			return std::weak_ptr<CGameObject>();
		return mWorldObjMap[ID](Coord, OnFocus, ChapterLevel);
	}
	std::shared_ptr<CGameDefinition> InstantiateDef(const int ID, const std::weak_ptr<CGameClass>& Owner)
	{
		if (!mDefMap.contains(ID))
			return std::weak_ptr<CGameDefinition>().lock();
		return mDefMap[ID](Owner);
	}
	const std::string& GetName(const int ID)
	{
		return mNameMap[ID];
	}

	void MakeWithData(class CGameData* GData);

private:
	void MakeChapter(class CChapterGData* GData, bool IsFocused = false);
};


