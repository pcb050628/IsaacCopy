#include "BossGemini.h"

#include "LogManager.h"
#include "TimeManager.h"

#include "../Manager/GameRuleManager.h"
#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderBox2D.h"
#include "World/ColliderSphere2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"
#include "World/SoundComponent.h"

#include "../Component/RigidBodyComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/RouteMaker.h"
#include "../Component/TearShooter.h"

REGISTER_GAMEOBJCLASS(CBossGemini, "Gemini", EObjectType::Monster)

CBossGemini::CBossGemini()
{
}

CBossGemini::~CBossGemini()
{
}

bool CBossGemini::Init()
{
    if (!CBoss::Init())
        return false;

    mContusionRigidbody = CreateComponent<CRigidBodyComponent>("Root");
    mContusionFullBodyMesh = CreateComponent<CMeshComponent>("ContusionFullBodyMesh");
    mContusionHeadMesh = CreateComponent<CMeshComponent>("ContusionHeadMesh");
    mContusionBodyMesh = CreateComponent<CMeshComponent>("ContusionBodyMesh");
    mContusionFullBodyAnimator = CreateComponent<CAnimation2DComponent>("ContusionFullBodyAnimator");
    mContusionHeadAnimator = CreateComponent<CAnimation2DComponent>("ContusionHeadAnimator");
    mContusionBodyAnimator = CreateComponent<CAnimation2DComponent>("ContusionBodyAnimator");
    mContusionHitBox = CreateComponent<CColliderSphere2D>("ContusionHit");
    mContusionHurtBox = CreateComponent<CColliderBox2D>("ContusionHurt");
    mRouteMaker = CreateComponent<CRouteMaker>("RouteMaker");
    if (mContusionRigidbody.expired() || 
        mContusionFullBodyMesh.expired() || mContusionFullBodyAnimator.expired() ||
        mContusionHeadMesh.expired() || mContusionHeadAnimator.expired() ||
        mContusionBodyMesh.expired() || mContusionBodyAnimator.expired() ||
        mContusionHitBox.expired() || mContusionHurtBox.expired() || mRouteMaker.expired())
        return false;

    mSutureRigidbody = CreateComponent<CRigidBodyComponent>("SutureRB");
    mSutureMesh = CreateComponent<CMeshComponent>("SutureMesh", "SutureRB");
    mSutureAnimator = CreateComponent<CAnimation2DComponent>("SutureAnimator", "SutureRB");
    mSutureHitBox = CreateComponent<CColliderSphere2D>("SutureHit", "SutureRB");
    mSutureHurtBox = CreateComponent<CColliderBox2D>("SutureHurt", "SutureRB");
    mShooter = CreateComponent<CTearShooter>("Shooter", "SutureRB");
    if (mSutureRigidbody.expired() || mSutureMesh.expired() || mSutureAnimator.expired() || mSutureHitBox.expired() || mSutureHurtBox.expired() || mShooter.expired())
        return false;

    for (int i = 0; i < 7; ++i)
    {
        std::shared_ptr<CSpriteComponent> sprite = CreateComponent<CSpriteComponent>("Sprite" + std::to_string(i + 1)).lock();
        assert(sprite && "스프라이트 컴포넌트 생성 실패");
        sprite->SetMesh("TexRect"); sprite->SetShader("Sprite2D"); sprite->SetRenderLayer("Body");
        sprite->SetWorldScale(25.f, 25.f);
        sprite->SetSpriteData("Gemini_Rope");
        mRopeVec.push_back(sprite);
    }

    //루트에서 위치정보 분리
    mSutureRigidbody.lock()->SetInheritPos(false);

    //메시 컴포넌트 설정 및 애니메이션 컴포넌트와 연결
    std::shared_ptr<CMeshComponent> cFullBody = mContusionFullBodyMesh.lock();
    std::shared_ptr<CMeshComponent> cHead= mContusionHeadMesh.lock();
    std::shared_ptr<CMeshComponent> cBody = mContusionBodyMesh.lock();
    std::shared_ptr<CAnimation2DComponent> cFullBodyAnimator = mContusionFullBodyAnimator.lock();
    std::shared_ptr<CAnimation2DComponent> cHeadAnimator = mContusionHeadAnimator.lock();
    std::shared_ptr<CAnimation2DComponent> cBodyAnimator = mContusionBodyAnimator.lock();
    cFullBody->SetMesh("TexRect"); cFullBody->SetShader("Animation2D"); cFullBody->SetRenderLayer("Head");
    cHead->SetMesh("TexRect"); cHead->SetShader("Animation2D"); cHead->SetRenderLayer("Head");
    cBody->SetMesh("TexRect"); cBody->SetShader("Animation2D"); cBody->SetRenderLayer("Body");

    cFullBodyAnimator->SetUpdateComponent(mContusionFullBodyMesh);
    cHeadAnimator->SetUpdateComponent(mContusionHeadMesh);
    cBodyAnimator->SetUpdateComponent(mContusionBodyMesh);

    std::shared_ptr<CMeshComponent> sMesh = mSutureMesh.lock();
    sMesh->SetMesh("TexRect"); sMesh->SetShader("Animation2D"); sMesh->SetRenderLayer("Head");
    std::shared_ptr<CAnimation2DComponent> sAnimator = mSutureAnimator.lock();
    sAnimator->SetUpdateComponent(mSutureMesh);
    
    //애니메이션 할당하기
    cFullBodyAnimator->AddAnimation("Contusion_Full_Body_Breathing", 1.f, 1.f, true); cFullBody->SetRenderEnable(false);
    cHeadAnimator->AddAnimation("Contusion_Head_Walk", 0.7f, 1.f, true);
    cBodyAnimator->AddAnimation("Contusion_Body_Walk_V", 1.f, 1.f, true); cBodyAnimator->AddAnimation("Contusion_Body_Walk_H", 1.f, 1.f, true);
    cFullBodyAnimator->Stop();

    sAnimator->AddAnimation("Suture_Shoot");
    sAnimator->AddAnimation("Suture_Move", 1.f, 1.f, true);
    sAnimator->SetFinishFunction("Suture_Shoot", this, &CBossGemini::Shoot);
    sAnimator->Stop(true);

    //허트 박스 설정
    std::shared_ptr<CColliderSphere2D> cHit = mContusionHitBox.lock();
    std::shared_ptr<CColliderSphere2D> sHit = mSutureHitBox.lock();

    std::shared_ptr<CColliderBox2D> cHurt = mContusionHurtBox.lock();
    std::shared_ptr<CColliderBox2D> sHurt = mSutureHurtBox.lock();

    cHurt->SetDebugDraw(true);
    cHurt->SetBoxSize(80.f, 80.f);
    cHurt->SetRelativePos(0.f, 20.f);

    cHurt->SetCollisionProfile("Monster"); cHurt->SetBeginOverlapFunc(this, &CBossGemini::OnContusionHurtOverlap);
    sHurt->SetCollisionProfile("Monster"); sHurt->SetBeginOverlapFunc(this, &CBossGemini::OnSutureHurtOverlap);

    cHit->SetCollisionProfile("Monster_ContactHit");
    sHit->SetCollisionProfile("Monster_ContactHit");

    //위치 및 크기 조정
    cFullBody->SetRelativePos(0.f, 20.f);
    cFullBody->SetWorldScale(100.f, 100.f);
    cHead->SetRelativePos(FVector2(0, 35.f));
    //cBody->SetRelativePos(FVector2(0, .f));
    cHead->SetWorldScale(FVector2(85.f, 85.f));
    cBody->SetWorldScale(FVector2(60.f, 60.f));

    sMesh->SetWorldScale(60.f, 60.f);

    //기능 컴포넌트 설정
    mContusionRigidbody.lock()->SetLimit(200.f);
    mSutureRigidbody.lock()->SetLimit(1000.f);
    mRouteMaker.lock()->SetChapter(mChapter);
    mRouteMaker.lock()->SetRoom(mRoomOwner);

    mShooter.lock()->SetCenterComponent(mSutureRigidbody);
    FTearAttribute tear;
    tear.Size = 10.f;
    tear.Damage = 1.f;
    tear.Speed = 10.f;
    tear.Range = 1000.f;
    tear.Height = 0.f;
    tear.knockback = 10.f;
    mShooter.lock()->UpdateTearAttributeData(tear);

    mShooterTimerHandle = CTimeManager::SetTimer(5.f, true, this, &CBossGemini::ShootToPlayer).GetID();
    float timeOffset = CGameRuleManager::GetInst()->GenerateRandomF() * 10 - 2;
    CTimeManager::SetTimer(mBreathingTime + timeOffset, false, this, &CBossGemini::StartBreathing).GetID();

    return true;
}

void CBossGemini::Update(float DeltaTime)
{
    UpdateContusion(DeltaTime);
    UpdateSuture(DeltaTime);
    UpdateRopePosition();

    CGameObject::Update(DeltaTime);
}

void CBossGemini::Reset(bool HardReset)
{
}

void CBossGemini::GetHit(std::weak_ptr<CGameObject> From)
{
}

void CBossGemini::MoveToPlayer()
{
    if (mbIsAttached)
    {
        FVector2 coord = mRouteMaker.lock()->MakeRoute();
        if (-FVector2::One == coord)
        {

        }
        else
        {
            FVector3 dir = mRoomOwner.lock()->CoordToWorldPos(coord) - mContusionRigidbody.lock()->GetWorldPos();
            dir.Normalize();
            mContusionRigidbody.lock()->AddForce(dir * 100.f);
            if (fabs(dir.x) > fabs(dir.y))
            {
                mContusionBodyAnimator.lock()->ChangeAnimation("Contusion_Body_Walk_H");
                bool symmetry = dir.x < 0;
				mContusionBodyAnimator.lock()->SetSymmetry("Contusion_Body_Walk_H", symmetry);
            }
            else
            {
                mContusionBodyAnimator.lock()->ChangeAnimation("Contusion_Body_Walk_V");
            }
        }
    }
    else
    {

    }
}

void CBossGemini::UpdateContusion(float DeltaTime)
{
    if (!mbIsBreathing)
    {
        MoveToPlayer();
    }
}

void CBossGemini::StartBreathing()
{
    mbIsBreathing = true;

    mContusionHeadMesh.lock()->SetRenderEnable(false);
    mContusionBodyMesh.lock()->SetRenderEnable(false);
    mContusionFullBodyMesh.lock()->SetRenderEnable(true);

    mContusionFullBodyAnimator.lock()->Play(true);
    CTimeManager::SetTimer(5.f, false, this, &CBossGemini::BreathingEnd).GetID();
}

void CBossGemini::BreathingEnd()
{
    mContusionHeadMesh.lock()->SetRenderEnable(true);
    mContusionBodyMesh.lock()->SetRenderEnable(true);
    mContusionFullBodyMesh.lock()->SetRenderEnable(false);

    mContusionFullBodyAnimator.lock()->Stop(true);
    
    mbIsBreathing = false;
    float timeOffset = CGameRuleManager::GetInst()->GenerateRandomF() * 20 - 10;
    CTimeManager::SetTimer(mBreathingTime + timeOffset, false, this, &CBossGemini::StartBreathing).GetID();
    LOG_DEBUG("시간 오프셋-", std::to_string(timeOffset));
}

void CBossGemini::OnContusionHurtOverlap(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
}

void CBossGemini::UpdateSuture(float DeltaTime)
{
    if (mbIsAttached)
    {
        FollowContusion();
        //몇초 마다 Shoottoplayer 호출하기
    }
    else
    {

    }
}

void CBossGemini::FollowContusion()
{
    FVector3 contusion = mContusionRigidbody.lock()->GetWorldPos();
    FVector3 suture = mSutureRigidbody.lock()->GetWorldPos();
    float dist = contusion.Distance(suture);
    FVector3 dir = contusion - suture;
    dir.Normalize();
    mSutureRigidbody.lock()->SetVelocity(dir * 25.f * (dist * 0.03f));
}

void CBossGemini::ShootToPlayer()
{
    FVector3 player = mChapter.lock()->GetPlayerCharacter().lock()->GetWorldPos();
    FVector3 suture = mSutureRigidbody.lock()->GetWorldPos();
    float dist = player.Distance(suture);

    if (dist > 10000.f)
        return;

    mSutureAnimator.lock()->ChangeAnimation("Suture_Shoot");
    mSutureAnimator.lock()->Play(true);
}

void CBossGemini::Shoot()
{
    FVector3 player = mChapter.lock()->GetPlayerCharacter().lock()->GetWorldPos();
    FVector3 suture = mSutureRigidbody.lock()->GetWorldPos();
    FVector3 dir = player - suture;
    dir.Normalize();

    std::shared_ptr<CTearShooter> shooter = mShooter.lock();
    FTearAttribute tear = shooter->GetTearAttribute();
    tear.Direction = FVector2(dir.x, dir.y);
    shooter->SetTearAttribute(tear);
    shooter->Fire(mSutureRigidbody.lock()->GetWorldPos(), true);
    mSutureAnimator.lock()->Stop(true);
}

void CBossGemini::OnSutureHurtOverlap(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
}

void CBossGemini::UpdateRopePosition()
{
    FVector3 contusion = mContusionRigidbody.lock()->GetWorldPos();
    FVector3 suture = mSutureRigidbody.lock()->GetWorldPos();
    FVector3 dir = contusion - suture;
    dir.Normalize();
    int length = mRopeVec.size();
    int dist = contusion.Distance(suture) / length;
    for (int i = 0; i < length; ++i)
    {
        FVector3 offset = dir * dist * (i + 1);
        mRopeVec[i].lock()->SetWorldPos(suture + offset);
    }
}
