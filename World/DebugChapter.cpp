#include "DebugChapter.h"

#include "Manager/GameClassContainer.h"

CDebugChapter::CDebugChapter()
{
	mChapterLevel = -1;
}

CDebugChapter::~CDebugChapter()
{
}

bool CDebugChapter::Init()
{
	CWorld::Init();
	/*if (!CChapter::Init())
		return false;*/

	mRoomRowMax = 10;
	mRoomColMax = 10;

	CGameClassContainer::GetInst()->Instantiate(10, FVector2(3, 3));

	return true;
}

void CDebugChapter::Update(float DeltaTime)
{
	CChapter::Update(DeltaTime);
}
