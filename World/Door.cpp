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

#include "Chapter.h"
#include "Component/RigidBodyComponent.h"
#include "Component/SpriteComponent.h"

#include "Base/Roombase.h"
#include "Base/Unitbase.h"

CDoor::CDoor()
{
}

CDoor::CDoor(const CDoor& src)
	:CActor(src)
{
}

CDoor::CDoor(CDoor&& src) noexcept
	:CActor(std::move(src))
{
}

CDoor::~CDoor()
{
}

bool CDoor::Init()
{
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

	frame->SetWorldScale(100.f, 100.f);
	//layer2->SetRelativePos(-10.f, 0);
	//layer3->SetRelativePos(10.f, 0);

	layer1->SetRenderState(0, "StencilMaskWrite");
	layer2->SetRenderState(0, "StencilMaskApply");
	layer3->SetRenderState(0, "StencilMaskApply");

	frame->SetSpriteData("Door_Wooden_Frame");
	layer1->SetSpriteData("Door_Wooden_Layer_1");
	layer2->SetSpriteData("Door_Wooden_Layer_2_1");
	layer3->SetSpriteData("Door_Wooden_Layer_2_2");

	frame->SetRenderLayer("Obstacle");
	layer1->SetRenderLayer("Obstacle");
	layer2->SetRenderLayer("Obstacle");
	layer3->SetRenderLayer("Obstacle");

	layer1->SetRelativePos(-0.9f, 0);

	mLayer2Rigid.lock()->SetMass(1.f); mLayer2Rigid.lock()->SetUseGravity(false); mLayer2Rigid.lock()->SetMoveRoot(false);
	mLayer3Rigid.lock()->SetMass(1.f); mLayer3Rigid.lock()->SetUseGravity(false); mLayer3Rigid.lock()->SetMoveRoot(false);

	std::shared_ptr<CSoundManager> soundMgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
	mCloseSound = soundMgr->FindSound("Obstacle_door_close");
	mOpenSound = soundMgr->FindSound("Obstacle_door_open");

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
			std::dynamic_pointer_cast<CChapter>(mWorld.lock())->MoveRoom(mDirection);
		}
	}

	CActor::Update(DeltaTime);
}

void CDoor::Destroy()
{
	CActor::Destroy();
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

		mLayer2Rigid.lock()->AddForce(FVector3(-1, 0, 0) * 200.f);
		mLayer3Rigid.lock()->AddForce(FVector3(1, 0, 0) * 200.f);
	}
	else
	{
		mBoxColComp.lock()->SetCollisionProfile("Wall");
		mSound.lock()->mSound = mCloseSound.lock();

		mLayer2Rigid.lock()->SetVelocity(FVector3::Zero);
		mLayer3Rigid.lock()->SetVelocity(FVector3::Zero);
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

void CDoor::OnOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	if (Collider.expired() || Collider.lock()->GetOwner().expired())
	{
		LOG_DEBUG(GetName(), ": ", "Collider is not valid");
		return;
	}

	std::shared_ptr<CUnitbase> player = std::dynamic_pointer_cast<CUnitbase>(Collider.lock()->GetOwner().lock());
	if (false || !player)
	{
		//현재는 유닛으로 해두었는데 플레이어 클래스 만들면 플레이어인지 확인하게 바꾸기
		LOG_DEBUG(GetName(), ": ", "Door has Deteced wrong Actor");
		return;
	}

	LOG_DEBUG("Hit 발생 : ", Collider.lock()->GetOwner().lock()->GetName());
	if (mbIsOpen)
	{
		mPlayer = player;
	}
}

void CDoor::ExitOverlaps(std::weak_ptr<class CCollider> Collider)
{
	mPlayer.reset();
}
