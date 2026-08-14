#include "DebugChapter.h"

#include "LogManager.h"

#include "Manager/GameClassContainer.h"
#include "Data/GameDataManager.h"
#include "Data/GameObjectStructure.h"

#include "GameSystemActor.h"

#include "RenderManager.h"
#include "World/ColliderBox2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

#include "Data/AnimGData.h"

#include "ImGui/include/imgui.h"

CDebugChapter::CDebugChapter()
{
	mChapterLevel = -1;
}

CDebugChapter::~CDebugChapter()
{
}

bool CDebugChapter::Init()
{
	//CWorld::Init();

	//CRenderManager::GetInst()->SetState("AlphaBlend");
	//
	//mRoomRowMax = 10;
	//mRoomColMax = 10;
	//
	////카메라 
	//mChapterManagementActor = CreateActor<CChapterSystemActor>("GSA");
	//
	//GenerateWallAndDoor();
	/*std::dynamic_pointer_cast<CColliderBox2D>(mWalls[0].lock()->GetRootComponent().lock())->SetBoxSize(50.f, 700.f);
	std::dynamic_pointer_cast<CColliderBox2D>(mWalls[1].lock()->GetRootComponent().lock())->SetBoxSize(50.f, 700.f);
	std::dynamic_pointer_cast<CColliderBox2D>(mWalls[2].lock()->GetRootComponent().lock())->SetBoxSize(1300.f, 50.f);
	std::dynamic_pointer_cast<CColliderBox2D>(mWalls[3].lock()->GetRootComponent().lock())->SetBoxSize(1300.f, 50.f);*/

	debugMode = false;

	if (debugMode) //애니메이션 만들기
	{
		//애니메이션 만들때 명심할것
		//같은 부위인 애니메이션들의 사이즈는 항상 동일해야함
		animMaker.Init();
	}
	else //테스트 코드
	{
		if (!CChapter::Init())
			return false;
		CGameClassContainer::GetInst()->Instantiate(20, FVector2(5, 3));
		CGameClassContainer::GetInst()->Instantiate(31, FVector2(7, 3));
		CGameClassContainer::GetInst()->Instantiate(81, FVector2(4, 3));
		
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(6, 3));
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(2, 2));
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(3, 4));
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(8, 5));
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(7, 1));
		
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(3, 4));
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(4, 2));
		CGameClassContainer::GetInst()->Instantiate(41, FVector2(9, 2));
		//20 gaper | 31 isaac
		//for (int y = 0; y < 7; ++y)
		//{
		//	for (int x = 0; x < 13; ++x)
		//	{
		//		CGameClassContainer::GetInst()->Instantiate(41, FVector2(x, y));
		//	}
		//}

		InitialSetting();

		CTimeManager::SetTimer(3.f, true, this, &CDebugChapter::CheckPlayerPos);
	}
	//if (!SetAnim("Gaper_Idle_Head", TEXT("Anim/Gaper_Idle_Head.txt"), true))
	//	return false;
	//if (!SetAnim("Gaper_Idle_Body", TEXT("Anim/Gaper_Idle_Body.txt")))
	//	return false;

	//CAnimGData d;
	//FAnimationData data;
	//data.Name = "Gaper_Body_Walk_H";
	//data.TextureName = "Gaper";
	//data.TexturePath = "Monster\\Gaper_.png";
	//data.Frames.reserve(9);
	//FFrameData f;
	//for (int i = 0; i < 4; i++)
	//{
	//	f.Start = FVector2(32.f * i, 0);
	//	f.Size = FVector2(32.f, 29.f);
	//	data.Frames.push_back(f);
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	f.Start = FVector2(32.f * i, 32.f);
	//	f.Size = FVector2(32.f, 29.f);
	//	data.Frames.push_back(f);
	//}
	//f.Start = FVector2(0, 64.f);
	//f.Size = FVector2(32.f, 29.f);
	//data.Frames.push_back(f);
	//f.Start = FVector2(32.f, 64.f);
	//f.Size = FVector2(32.f, 29.f);
	//data.Frames.push_back(f);
	//
	//d.ContainData(data);
	//d.WriteData(TEXT("Anim\\Gaper_Body_Walk_H"));

	//애니메이션 만들 때 필요한것
	//1. 애니메이션 이름, 텍스쳐 이름, 텍스쳐 경로, 데이터 저장 경로
	//2. 저장할 객체는 지역변수로 해도 문제없음
	//다시
	//필요한것
	//1. 메뉴만으로 충분함
	//2. 초기 제작용 문자열 4개
	//3. 프레임 제작용 실수 4개
	//imgui 붙이는건 얼마나 귀찮지?

	//

	return true;
}

void CDebugChapter::Update(float DeltaTime)
{
	if(debugMode)
		animMaker.Update();
	else
		CChapter::Update(DeltaTime);
}

void CDebugChapter::CheckPlayerPos()
{
	auto player = mPlayerCharacter.lock();
	if (!player)
		return;

	FVector3 pos = player->GetWorldPos();
	LOG_DEBUG("PLAYER POS: ", std::to_string(pos.x), ", ", std::to_string(pos.y), ", ", std::to_string(pos.z));
}
