#include "Unitbase.h"

#include "../Component/RigidBodyComponent.h"
#include "World/MeshComponent.h"
#include "World/ColliderSphere2D.h"

CUnitbase::CUnitbase(EObjectType Type)
	:CGameObject(Type)
{
}

CUnitbase::CUnitbase(const CUnitbase& src)
	:CGameObject(src)
{
}

CUnitbase::CUnitbase(CUnitbase&& src) noexcept
	:CGameObject(std::move(src))
{
}

CUnitbase::~CUnitbase()
{
}

bool CUnitbase::Init()
{
	mRigidBody = CreateComponent<CRigidBodyComponent>("Root");
	if (mRigidBody.expired())
		return false;

	mHeadMesh = CreateComponent<CMeshComponent>("MHead");
	mBodyMesh = CreateComponent<CMeshComponent>("MBody");
	if (mHeadMesh.expired() || mBodyMesh.expired())
		return false;

	//몸 머리 오프셋 넣기 - SetRelativePos, SetRelativeScale

	mHurtBox = CreateComponent<CColliderSphere2D>("HurtBox");
	if (mHurtBox.expired())
		return false;

	std::shared_ptr<CColliderSphere2D> hitbox = mHurtBox.lock();
	hitbox->SetBeginOverlapFunc(this, &CUnitbase::OnHurtOverlaps);
	hitbox->SetEndOverlapFunc(this, &CUnitbase::ExitHurtOverlaps);

	//히트 박스 크기 조정

	mActorTag = "Unit";
	return true;
}

void CUnitbase::Update(float DeltaTime)
{
	CActor::Update(DeltaTime);
}

void CUnitbase::Destroy()
{
	CActor::Destroy();
}

void CUnitbase::Move(const FVector3& Force) const
{
	mRigidBody.lock()->SetVelocity(Force);
}

void CUnitbase::Fire()
{
}

void CUnitbase::Dead()
{
	mbIsDead = true;
}
