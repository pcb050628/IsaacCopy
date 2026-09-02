#include "DebugChapter.h"

#include "Device.h"
#include "LogManager.h"

#include "Manager/GameClassContainer.h"
#include "Data/GameDataManager.h"
#include "Data/GameObjectStructure.h"

#include "GameSystemActor.h"

#include "World/Input.h"
#include "RenderManager.h"
#include "World/ColliderBox2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"
#include "World/FontRenderer.h"

#include "Door.h"

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
	dataType = EGDataType::Sprite;
	
	if (debugMode) //애니메이션 만들기
	{
		//애니메이션 만들때 명심할것
		//같은 부위인 애니메이션들의 사이즈는 항상 동일해야함
		switch (dataType)
		{
		case EGDataType::Anim:
			animMaker.Init();
			break;
		case EGDataType::Sprite:
			spriteMaker.Init();
			break;
		case EGDataType::Room:
			roomMaker.Init();
			break;
		default:
			return false;
		}
		//roomMaker.Init();
	}
	else //테스트 코드
	{
		if (!CChapter::Init())
			return false;
		//CGameClassContainer::GetInst()->Instantiate(20, FVector2(5, 3));
		//CGameClassContainer::GetInst()->Instantiate(81, FVector2(4, 3));
		//
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(6, 3));
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(2, 2));
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(3, 4));
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(8, 5));
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(7, 1));
		//
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(3, 4));
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(4, 2));
		//CGameClassContainer::GetInst()->Instantiate(41, FVector2(9, 2));
		//20 gaper | 31 isaac
		//for (int y = 0; y < 7; ++y)
		//{
		//	for (int x = 0; x < 13; ++x)
		//	{
		//		CGameClassContainer::GetInst()->Instantiate(41, FVector2(x, y));
		//	}
		//}

		mInput->AddBindKey("DebugKey", VK_NUMPAD0);
		mInput->SetBindFunction("DebugKey", EInputType::Press, this, &CDebugChapter::DebugDrawTogle);

		RegisterCharacter(31);
		CGameClassContainer::GetInst()->Instantiate(40, FVector2(1, 1));

		/*std::weak_ptr<CActor> actor = CreateActor<CActor>("TEST");
		std::weak_ptr<CFontRenderer> font = actor.lock()->CreateComponent<CFontRenderer>("Root");
		font.lock()->SetSize(400.f, 400.f);
		font.lock()->SetFontSize(32.f);
		font.lock()->SetFont("GameDefault");
		font.lock()->SetText(L"Another One Bites The Dust");
		FResolution resol = CDevice::GetInst()->GetResolution();
		font.lock()->SetRenderPos(resol.Width / 2, resol.Height /2);*/

		InitialSetting();

		//CTimeManager::SetTimer(3.f, true, this, &CDebugChapter::CheckPlayerPos);
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
	if (debugMode)
	{
		switch (dataType)
		{
		case EGDataType::Anim:
			animMaker.Update();
			break;
		case EGDataType::Sprite:
			spriteMaker.Update();
			break;
		case EGDataType::Room:
			roomMaker.Update();
			break;
		}
	}
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

void CDebugChapter::DebugDrawTogle()
{
	static bool val = true;

	val = !val;
	CRenderManager::GetInst()->SetLayerEnable(10, val);
}
