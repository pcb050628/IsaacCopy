#include "Door.h"

#include "LogManager.h"

#include "World/ColliderBox2D.h"
#include "World/MeshComponent.h"

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
	if (mBoxColComp.expired())
		return false;

	mMeshComp = CreateComponent<CMeshComponent>("MeshComp");
	if (mMeshComp.expired())
		return false;

	std::shared_ptr<CColliderBox2D> box = mBoxColComp.lock();
	box->SetCollisionProfile("Door");
	box->SetBeginOverlapFunc(this, &CDoor::OnOverlaps);
	box->SetEndOverlapFunc(this, &CDoor::ExitOverlaps);

	//메시 또는 애니메이션 초기화 코드 작성하기

	return true;
}

void CDoor::Update(float DeltaTime)
{
	if (!mPlayer.expired())
	{
		if (mPlayer.lock()->GetWorldPos().Distance(GetWorldPos()) <= 0.5f) //거리가 충분히 가까워졌다면
		{
			//이동하기
		}
	}
}

void CDoor::Destroy()
{
	CActor::Destroy();
}

void CDoor::SetOpen(bool Val)
{
	//나중에 작성할것
	//
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

	if(mbIsOpen)
		mPlayer = player;
	else
	{
		float dist;
		FVector3 aPos = player->GetWorldPos();
		FVector3 mPos = GetWorldPos();

		std::shared_ptr<CColliderBox2D> box = mBoxColComp.lock();
		std::shared_ptr<CCollider> col = Collider.lock();

		if (Normal.x != 0)
		{
			if (Normal.x > 0)
				dist = (box->GetMax().x - col->GetMin().x) + 1.f;
			else
				dist = -((box->GetMin().x - col->GetMax().x) + 1.f);
		}
		else
		{
			if (Normal.y > 0)
				dist = (box->GetMax().y - col->GetMin().y) + 1.f;
			else
				dist = -((box->GetMin().y - col->GetMax().y) + 1.f);
		}

		player->AddRelativePos(Normal * dist);
	}
}

void CDoor::ExitOverlaps(std::weak_ptr<class CCollider> Collider)
{
	mPlayer.reset();
}
