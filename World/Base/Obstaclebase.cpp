#include "Obstaclebase.h"

#include "World/ColliderBox2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

#include "../Chapter.h"
#include "Roombase.h"

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

	//셀 크기 챕터에서 가져올 수 잇게 바꾸기
	FVector2 cellsize = mChapter.lock()->GetFocusedRoom().lock()->GetRoomCellSize();

	auto box = mHitBox.lock();
	box->SetCollisionProfile("Obstacle");
	box->SetBoxSize(cellsize * 0.8f);
	box->SetDebugDraw(true);

	auto mesh = mMeshComp.lock();
	mesh->SetMesh("TexRect"); mesh->SetShader("Animation2D");
	mesh->SetRenderLayer("Obstacle");
	mesh->SetWorldScale(cellsize);

	auto animator = mAnimator.lock();
	animator->SetUpdateComponent(mMeshComp);

	return true;
}
