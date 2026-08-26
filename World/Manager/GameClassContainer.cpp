#include "GameClassContainer.h"
#include "../Data/GameData.h"
#include "../Data/GameObjectStructure.h"

#include "World/WorldManager.h"
#include "../Chapter.h"

#include "../Data/ChapterGData.h"
#include "../Data/RoomGData.h"

CGameClassContainer* CGameClassContainer::mInstance = nullptr;

CGameClassContainer::CGameClassContainer()
{

}
CGameClassContainer::~CGameClassContainer()
{

}

void CGameClassContainer::MakeWithData(CGameData* GData)
{
	switch (GData->GetType())
	{
	case EGDataType::Chapter:
		MakeChapter(dynamic_cast<CChapterGData*>(GData));
		break;
	case EGDataType::Room:
	case EGDataType::Player:
	default:
		assert("잘못된 접근: 챕터가 아닌 다른 데이터로 런 생성을 시도함");
		break;
	}
}

void CGameClassContainer::MakeChapter(CChapterGData* GData, bool IsFocused)
{
	FChapterData d = GData->GetData();
	std::weak_ptr<CChapter> chapter = CWorldManager::GetInst()->CreateWorld<CChapter>();
	for (int i = 0; i < d.Rooms.size(); ++i)
	{
		Instantiate(d.Rooms[i].ID, d.Rooms[i].Coord, IsFocused, d.ChapterLevel);
	}
}