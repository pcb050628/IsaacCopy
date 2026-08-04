#include "DebugChapter.h"

#include "Manager/GameClassContainer.h"
#include "Data/GameDataManager.h"
#include "Data/GameObjectStructure.h"

#include "GameSystemActor.h"

#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

#include "Data/AnimGData.h"

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

	mChapterManagementActor = CreateActor<CChapterSystemActor>("GSA");

	//CGameClassContainer::GetInst()->Instantiate(10, FVector2(3, 3));

	//if (!SetAnim("Gaper_Idle_Head", TEXT("Anim/Gaper_Idle_Head.txt"), true))
	//	return false;
	//if (!SetAnim("Gaper_Idle_Body", TEXT("Anim/Gaper_Idle_Body.txt")))
	//	return false;

	CAnimGData d;
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


	//d.Read(TEXT("Anim\\Gaper_Body_Walk_H"));
	//d.MakeAnim();
	//
	//std::shared_ptr<CActor> a = CreateActor<CActor>("Test").lock();
	//auto mesh = a->CreateComponent<CMeshComponent>("Root");
	//auto animator = a->CreateComponent<CAnimation2DComponent>("Animator").lock();
	//mesh.lock()->SetMesh("TexRect");
	//mesh.lock()->SetShader("Animation2D");
	//mesh.lock()->SetWorldScale(FVector2(200.f, 200.f));
	//
	//animator->SetUpdateComponent(mesh);
	//animator->AddAnimation(d.GetData().Name, 1.f, 1.f, true);

	return true;
}

void CDebugChapter::Update(float DeltaTime)
{
	CChapter::Update(DeltaTime);
}
