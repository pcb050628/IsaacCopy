#include "Door.h"

#include "LogManager.h"

#include "World/ColliderBox2D.h"
#include "World/SoundComponent.h"

#include "World/CollisionInfoManager.h"

#include "Asset/AssetManager.h"
#include "Asset/SoundManager.h"
#include "Data/GameDataManager.h"
#include "Data/GameObjectStructure.h"
#include "Data/SpriteGData.h"

#include "Manager/GameClassContainer.h"

#include "Chapter.h"
#include "Component/RigidBodyComponent.h"
#include "Component/SpriteComponent.h"

#include "Base/Roombase.h"
#include "Base/Unitbase.h"

REGISTER_GAMEOBJCLASS(CDoor, "Door", EObjectType::Door)

CDoor::CDoor()
	:CRoomMember(EObjectType::Door)
{
}

CDoor::CDoor(const CDoor& src)
	:CRoomMember(src)
{
}

CDoor::CDoor(CDoor&& src) noexcept
	:CRoomMember(std::move(src))
{
}

CDoor::~CDoor()
{
}

bool CDoor::Init()
{
	if (!CRoomMember::Init())
		return false;
	mBoxColComp = CreateComponent<CColliderBox2D>("Root");
	mFrameRenderer = CreateComponent<CSpriteComponent>("FrameSprite");

	mLayerRenderer1 = CreateComponent<CSpriteComponent>("LayerSprite", "FrameSprite");
	mLayer2Rigid = CreateComponent<CRigidBodyComponent>("Layer2Rigid", "FrameSprite");
	mLayer3Rigid = CreateComponent<CRigidBodyComponent>("Layer3Rigid", "FrameSprite");

	mLayerRenderer2 = CreateComponent<CSpriteComponent>("LayerSprite", "Layer2Rigid");
	mLayerRenderer3 = CreateComponent<CSpriteComponent>("LayerSprite", "Layer3Rigid");
	mSound = CreateComponent<CSoundComponent>("DoorSound");
	if (mBoxColComp.expired()
		|| mFrameRenderer.expired() || mLayerRenderer1.expired() || mLayerRenderer2.expired() || mLayerRenderer3.expired()
		|| mSound.expired())
		return false;

	std::shared_ptr<CColliderBox2D> box = mBoxColComp.lock();
	box->SetBoxSize(100.f, 100.f);
	box->SetCollisionProfile("Wall");
	box->SetBeginOverlapFunc(this, &CDoor::OnOverlaps);
	box->SetEndOverlapFunc(this, &CDoor::ExitOverlaps);
	box->SetDebugDraw(true);

	//메시 또는 애니메이션 초기화 코드 작성하기

	std::shared_ptr<CSpriteComponent> frame = mFrameRenderer.lock();
	std::shared_ptr<CSpriteComponent> layer1 = mLayerRenderer1.lock();
	std::shared_ptr<CSpriteComponent> layer2 = mLayerRenderer2.lock();
	std::shared_ptr<CSpriteComponent> layer3 = mLayerRenderer3.lock();
	frame->SetMesh("TexRect"); frame->SetShader("Sprite2D");
	layer1->SetMesh("TexRect"); layer1->SetShader("Sprite2D");
	layer2->SetMesh("TexRect"); layer2->SetShader("Sprite2D");
	layer3->SetMesh("TexRect"); layer3->SetShader("Sprite2D");

	frame->SetWorldScale(160.f, 160.f);
	frame->SetRelativePos(0, 15.f);

	layer1->SetRenderState(0, "StencilMaskWrite");
	layer2->SetRenderState(0, "StencilMaskApply");
	layer3->SetRenderState(0, "StencilMaskApply");

	frame->SetRenderLayer("Door");
	layer1->SetRenderLayer("Obstacle");
	layer2->SetRenderLayer("Obstacle");
	layer3->SetRenderLayer("Obstacle");

	layer1->SetRelativePos(-0.9f, -3.f);

	mLayer2Rigid.lock()->SetMass(1.f); mLayer2Rigid.lock()->SetLimit(5000.f); mLayer2Rigid.lock()->SetUseGravity(false); mLayer2Rigid.lock()->SetMoveRoot(false);
	mLayer3Rigid.lock()->SetMass(1.f); mLayer3Rigid.lock()->SetLimit(5000.f); mLayer3Rigid.lock()->SetUseGravity(false); mLayer3Rigid.lock()->SetMoveRoot(false);

	std::shared_ptr<CSoundManager> soundMgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
	mCloseSound = soundMgr->FindSound("Obstacle_door_close");
	mOpenSound = soundMgr->FindSound("Obstacle_door_open");

	SetDoorFrameType(ERoomType::Normal);

	return true;
}

void CDoor::Update(float DeltaTime)
{
	if (!mPlayer.expired())
	{
		if (mPlayer.lock()->GetWorldPos().Distance(GetWorldPos()) <= 15.f) //거리가 충분히 가까워졌다면
		{
			//이동하기
			LOG_DEBUG("다음 방으로 이동합니다.");
			assert(mChapter.lock() && "챕터가 할당되어있지 않습니다.");
			mChapter.lock()->MoveRoom(mDirection);
		}
	}
	else
	{
		mPlayer = std::dynamic_pointer_cast<CRoomMember>(mChapter.lock()->GetPlayerCharacter().lock());
	}

	CActor::Update(DeltaTime);
}

void CDoor::Destroy()
{
	CActor::Destroy();
}

void CDoor::Reset(bool HardReset)
{
	SetOpen(false);
}

void CDoor::SetDoorFrameType(ERoomType Type)
{
	std::string targetName;
	switch (Type)
	{
	case ERoomType::Normal:
		targetName = "Door_Wooden_"; //챕터에 따라서 기본 프레임 이름은 변경해주기
		break;
	case ERoomType::Boss:
		targetName = "Door_Boss_";
		break;
	case ERoomType::Shop:
		targetName = "Door_Shop_"; 
		break;
	case ERoomType::Treasure:
		targetName = "Door_Treasure_"; 
		break;
	case ERoomType::Start:
		break;
	case ERoomType::Angel:
		break;
	case ERoomType::Devil:
		break;
	case ERoomType::Secret:
		break;
	case ERoomType::End:
		break;
	default:
		break;
	}

	mFrameRenderer.lock()->SetSpriteData(targetName + "Frame");
	mLayerRenderer1.lock()->SetSpriteData(targetName + "Layer_1");
	mLayerRenderer2.lock()->SetSpriteData(targetName + "Layer_2_1");
	mLayerRenderer3.lock()->SetSpriteData(targetName + "Layer_2_2");
}

void CDoor::SetOpen(bool Val)
{
	mLayer2Rigid.lock()->SetRelativePos(0, 0);
	mLayer3Rigid.lock()->SetRelativePos(0, 0);

	mbIsOpen = Val;
	if(mbIsOpen)
	{
		mBoxColComp.lock()->SetCollisionProfile("Door");
		mSound.lock()->mSound = mOpenSound.lock();

		mLayer2Rigid.lock()->AddForce(FVector3(-1, 0, 0) * 350.f);
		mLayer3Rigid.lock()->AddForce(FVector3(1, 0, 0) * 350.f);
		mInfo.state = EOpenState::Open;
	}
	else
	{
		mBoxColComp.lock()->SetCollisionProfile("Wall");
		mSound.lock()->mSound = mCloseSound.lock();

		mLayer2Rigid.lock()->SetVelocity(FVector3::Zero);
		mLayer3Rigid.lock()->SetVelocity(FVector3::Zero);
		mInfo.state = EOpenState::Closed;
	}
	mSound.lock()->Play();
}

void CDoor::SetBoxSize(FVector2 size)
{
	mBoxColComp.lock()->SetBoxSize(size);
}

void CDoor::SetBoxSize(float x, float y)
{
	mBoxColComp.lock()->SetBoxSize(x, y);
}

void CDoor::MetRequirement(EOpenRequirement requirement)
{
	if (requirement == mInfo.remainRequirement)
	{
		SetOpen(true);
	}
	else
	{
		switch (mInfo.remainRequirement)
		{
		case EOpenRequirement::Key2:
			if (EOpenRequirement::Key == requirement)
				mInfo.remainRequirement = EOpenRequirement::Key;
			break;
		case EOpenRequirement::Blast2:
			if (EOpenRequirement::Blast == requirement)
				mInfo.remainRequirement = EOpenRequirement::Blast;
			break;
		}
	}
}

void CDoor::SetDoorOpenInfo(FOpenInfo state)
{
	mInfo = state;
	switch (mInfo.state)
	{
	case EOpenState::Closed:
		SetOpen(false);
		break;
	case EOpenState::Blasted: //이미지 변경하기
	case EOpenState::Open:
		SetOpen(true);
		break;
	}
}

void CDoor::OnOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{

}

void CDoor::ExitOverlaps(std::weak_ptr<class CCollider> Collider)
{
}
