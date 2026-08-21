#include "Obstaclebase.h"

#include "World/ColliderBox2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

CObstaclebase::CObstaclebase(EObstacleType ObstacleType)
	:CRoomMember(EObjectType::Obstacle), mObstacleType(ObstacleType)
{
}

CObstaclebase::CObstaclebase(const CObstaclebase& src)
	:CRoomMember(src), mObstacleType(src.mObstacleType)
{
}

CObstaclebase::CObstaclebase(CObstaclebase&& src) noexcept
	:CRoomMember(std::move(src)), mObstacleType(src.mObstacleType)
{
}

CObstaclebase::~CObstaclebase()
{
}

bool CObstaclebase::Init()
{
	if (!CRoomMember::Init())
		return false;

	mHitBox = CreateComponent<CColliderBox2D>("Root");
	mMeshComp = CreateComponent<CMeshComponent>("Root");
	mAnimator = CreateComponent<CAnimation2DComponent>("Root");
	if (mHitBox.expired() || mMeshComp.expired() || mAnimator.expired())
		return false;

	auto box = mHitBox.lock();
	box->SetCollisionProfile("Obstacle");
	box->SetBoxSize(50.f, 50.f);
	box->SetDebugDraw(true);

	auto mesh = mMeshComp.lock();
	mesh->SetMesh("TexRect"); mesh->SetShader("Animation2D");
	mesh->SetRenderLayer("Obstacle");
	mesh->SetWorldScale(75.f, 75.f);

	auto animator = mAnimator.lock();
	animator->SetUpdateComponent(mMeshComp);

	return true;
}
