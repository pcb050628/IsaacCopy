#include "DefaultRoom.h"
#include "../Base/Unitbase.h"

#include "../Manager/GameClassContainer.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/AnimationManager.h"
#include "World/WorldManager.h"
#include "../Chapter.h"

#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

REGISTER_GAMECLASS(CDefaultRoom, "Room_Default", EObjectType::Room);

CDefaultRoom::CDefaultRoom()
	:CRoombase(ERoomType::Normal, ERoomShape::Normal)
{
	
}

CDefaultRoom::~CDefaultRoom()
{
}

bool CDefaultRoom::Init()
{
	if (!CRoombase::Init())
		return false;

	//메시에 텍스쳐 넣기

	mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundLT"));
	//mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundRT"));
	//mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundLB"));
	//mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundRT"));

	mBackgroundMesh[0].lock()->SetRelativePos(FVector2(-325.f, 175.f));
	mBackgroundMesh[0].lock()->SetRelativeScale(FVector2(650.f, 350.f));
	mBackgroundMesh[0].lock()->SetMesh("TexRect"); mBackgroundMesh[0].lock()->SetShader("Animation2D");
	std::wstring path = CPathManager::FindPath("Texture");
	path += TEXT("Room\\Basement.png");
	mBackgroundMesh[0].lock()->AddTextureFullPath(0, "Basement", path.c_str());

	//애니메이션으로 uv 설정중
	auto mgr = CAssetManager::GetInst()->GetSubManager<CAnimationManager>(EAssetType::Animation2D);
	mgr->CreateAnimation("Background_1");
	mgr->SetTexture("Background_1", "Basement");
	mgr->SetAnimationTextureType("Background_1", EAnimation2DTextureType::SpriteSheet);
	mgr->AddFrame("Background_1", FVector2::Zero, FVector2(234.f, 155.f));

	auto animator = CreateComponent<CAnimation2DComponent>("Animator").lock();
	animator->SetUpdateComponent(mBackgroundMesh[0]);
	animator->AddAnimation("Background_1", 1.f, 1.f, true);

	return true;
}

void CDefaultRoom::Update(float DeltaTime)
{
	CRoombase::Update(DeltaTime);
}

void CDefaultRoom::Destroy()
{
	CActor::Destroy();
}

void CDefaultRoom::OnEnterRoom()
{
}

void CDefaultRoom::WinRoom()
{
	mbIsRoomWin = true;
	//보상 스폰하기
}

void CDefaultRoom::OnExitRoom()
{
}

bool CDefaultRoom::WinCheck()
{
	bool flag = false;
	for (std::weak_ptr<CUnitbase> unit : mUnits)
	{
		if (unit.lock()->IsEnable())
		{
			return false;
		}
	}
	return true;
}