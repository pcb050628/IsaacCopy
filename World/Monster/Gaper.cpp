#include "Gaper.h"

#include "World/Animation2DComponent.h"
#include "World/ColliderBox2D.h"
#include "World/ColliderSphere2D.h"

#include "../Manager/GameClassContainer.h"

#include "../Chapter.h"
#include "../Base/Roombase.h"
#include "../Component/RigidBodyComponent.h"

REGISTER_GAMECLASS(CGaper, "Gaper", EObjectType::Monster);

CGaper::CGaper()
{
}

CGaper::CGaper(const CGaper& src)
{
}

CGaper::CGaper(CGaper&& src) noexcept
{
}

CGaper::~CGaper()
{
}

bool CGaper::Init()
{
	if (!CWalker::Init())
		return false;

	//머리 //거지같은 rapidjson 잘못써서 다시 만들어야함
	//머리 다시 만들기
	//있는거 그대로 이어 붙이는게 아니라 애니메이션 효과를 생각하면서 붙여야함
	//.처음->처음(찌그러짐)->눈뜸(세로로 살짝 길어짐)->뜸 순으로 만들어야함
	if (!AddAnim("Gaper_Head_Close", TEXT("Gaper_Head_Close"), true))
		return false;
	if (!AddAnim("Gaper_Head_Open_2", TEXT("Gaper_Head_Open_2"), true, 0.6f))
		return false;

	//몸
	if (!AddAnim("Gaper_Body_Walk_V", TEXT("Gaper_Body_Walk_V"), false, 0.8f ,1.f, true))
		return false;
	if (!AddAnim("Gaper_Body_Walk_H", TEXT("Gaper_Body_Walk_H"), false, 1.f, 1.f, true))
		return false;

	//현재 몬스터의 머리 몸 위치 비율
	//mHeadMesh.lock()->SetRelativePos(FVector2(0, 44.f));
	//mBodyMesh.lock()->SetRelativePos(FVector2(0, 9.f));
	//
	//mHeadMesh.lock()->SetRelativeScale(FVector2(7.f, 7.f));
	//mBodyMesh.lock()->SetRelativeScale(FVector2(5.5f, 5.5f));

	mBody.lock()->Stop();
	//mBody.lock()->ChangeAnimation("Gaper_Body_Walk_H");
	mHead.lock()->ChangeAnimation("Gaper_Head_Open_2");

	mHurtBox.lock()->SetRadius(20.f);
	mHurtBox.lock()->SetDebugDraw(true);
	mHurtBox.lock()->SetRenderLayer("Debug");

	mRigidBody.lock()->SetLimit(100.f);

	//다시 
	//게이퍼의 행동들 역할들과 연관지어 분리하기
	//.이동 -> 애니메이션 변경 + Rb로 방향 보내기
	//.대기 -> 애니메이션 정지

	return true;
}

void CGaper::Update(float DeltaTime)
{
	if (mTarget.expired())
	{
		std::shared_ptr<CChapter> chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
		if (!chptr)
		{
			assert("월드 없이 생성된 객체가 있습니다.");
			return;
		}
		//유닛 가져오기
		mTarget = std::dynamic_pointer_cast<CUnitbase>(chptr->GetPlayerCharacter().lock());
	}
	else
	{
		if (mRoomOwner.expired())
		{
			assert("방 없이 존재하는 유닛이 업데이트를 받고 있음");
			return;
		}

		if (mRoomOwner.lock()->CanGetToPlayerCharacter(GetWorldPos()))
		{
			//플레이어에게 가기
			//1. 플레이어 방향 구하기
			//2. 해당 방향에 장애물이 있는지 확인하기
			// 방향에 뭔가 있는지 확인할때 좋은 방법은 
			// 여기서는 해당 좌표의 셀을 얻어오는 것이고
			// 범용성있게는 raycast 를 사용하는 것
			//3. 장애물이 있다면 다른 두번째로 가까운 방향으로 가기
			
			//일단 그냥 방향으로만 가게 해놨는데 이건 나중에 고치기

			FVector3 dir = mTarget.lock()->GetWorldPos() - GetWorldPos();
			dir.Normalize();
			mRigidBody.lock()->AddForce(dir * 100.f);
		}
		else
		{
			mHead.lock()->Clear();
			mBody.lock()->Clear();
		}
	}

	CWalker::Update(DeltaTime);
}

void CGaper::Destory()
{
}

void CGaper::Dead()
{
}

void CGaper::Reset(bool HardReset)
{

}

void CGaper::PlayBodyVerticalAnim()
{
	mBody.lock()->ChangeAnimation("Gaper_Body_Walk_V");
	bool symmetry = mBodyDirection.y > 0 ? true : false;
	mBody.lock()->SetSymmetry("Gaper_Body_Walk_V", symmetry);
	mBody.lock()->Play();
}

void CGaper::PlayBodyHorizontalAnim()
{
	mBody.lock()->ChangeAnimation("Gaper_Body_Walk_H");
	bool symmetry = mBodyDirection.x > 0 ? false : true;
	mBody.lock()->SetSymmetry("Gaper_Body_Walk_H", symmetry);
	mBody.lock()->Play();
}

void CGaper::PlayHeadVerticalAnim()
{
}

void CGaper::PlayHeadHorizontalAnim()
{
}

void CGaper::MoveToTarget()
{
}