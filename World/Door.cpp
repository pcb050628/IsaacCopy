#include "Door.h"

#include "LogManager.h"

#include "World/ColliderBox2D.h"
#include "World/MeshComponent.h"
#include "World/SoundComponent.h"
#include "World/Animation2DComponent.h"

#include "World/CollisionInfoManager.h"

#include "Asset/AssetManager.h"
#include "Asset/SoundManager.h"
#include "Asset/AnimationManager.h"
#include "Data/GameDataManager.h"
#include "Data/GameObjectStructure.h"
#include "Data/AnimGData.h"

#include "Chapter.h"
#include "Component/RigidBodyComponent.h"

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
	mMesh = CreateComponent<CMeshComponent>("Mesh");
	mAnimator = CreateComponent<CAnimation2DComponent>("Animator");
	mSound = CreateComponent<CSoundComponent>("DoorSound");
	if (mBoxColComp.expired() || mMesh.expired() || mAnimator.expired() || mSound.expired())
		return false;

	mMesh.lock()->SetWorldScale(100.f, 100.f);

	std::shared_ptr<CColliderBox2D> box = mBoxColComp.lock();
	box->SetBoxSize(100.f, 100.f);
	box->SetCollisionProfile("Wall");
	box->SetBeginOverlapFunc(this, &CDoor::OnOverlaps);
	box->SetEndOverlapFunc(this, &CDoor::ExitOverlaps);
	box->SetDebugDraw(true);

	//메시 또는 애니메이션 초기화 코드 작성하기

	std::shared_ptr<CMeshComponent> mesh = mMesh.lock();
	mesh->SetMesh("TexRect"); mesh->SetShader("Animation2D");

	auto mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
	if(!mgr->LoadDataFile<CAnimGData>("Door_Wooden_Frame", EGDataType::Anim, TEXT("Door_Wooden_Frame")))
		return false;
	if (!mgr->LoadDataFile<CAnimGData>("Door_Wooden_Layer", EGDataType::Anim, TEXT("Door_Wooden_Layer")))
		return false;
	auto frameData = mgr->FindData<CAnimGData>("Door_Wooden_Frame", EGDataType::Anim).lock();
	frameData->MakeAnim();
	auto layerData = mgr->FindData<CAnimGData>("Door_Wooden_Layer", EGDataType::Anim).lock();
	layerData->MakeAnim();

	std::shared_ptr<CAnimation2DComponent> animator = mAnimator.lock();
	animator->SetUpdateComponent(mMesh);
	animator->AddAnimation(frameData->GetData().Name);
	animator->AddAnimation(layerData->GetData().Name);

	mMesh.lock()->SetRenderLayer("Obstacle");

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
	mbIsOpen = Val;
	if(mbIsOpen)
	{
		mBoxColComp.lock()->SetCollisionProfile("Door");
		mSound.lock()->mSound = mOpenSound.lock();
	}
	else
	{
		mBoxColComp.lock()->SetCollisionProfile("Wall");
		mSound.lock()->mSound = mCloseSound.lock();
	}
	mSound.lock()->Play();
	//나중에 작성할것
	//애니메이션 출력
	//문열리는 애니메이션 만들어야하는데
	//이 거지같은 리소스가 시발 문이 양쪽이 떨어져서 만들어져 있고
	//메시 두개 올려서 스텐실 뷰 사용해야할듯
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
