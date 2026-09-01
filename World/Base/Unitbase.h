#pragma once
#include "RoomMember.h"
#include "../../GameObjectStructs.h"

class CUnitbase :
    public CRoomMember 
{
public:
    CUnitbase(EObjectType Type);
    CUnitbase(const CUnitbase& src);
    CUnitbase(CUnitbase&& src) noexcept;
    virtual ~CUnitbase();
    
protected:
    std::weak_ptr<class CRigidBodyComponent> mRigidBody; //root
    std::weak_ptr<class CMeshComponent> mHeadMesh;
    std::weak_ptr<class CMeshComponent> mBodyMesh;
    std::weak_ptr<class CAnimation2DComponent> mHead;
    std::weak_ptr<class CAnimation2DComponent> mBody;
    std::weak_ptr<class CColliderSphere2D> mHurtBox; //피격체 | 피격을 당하지 않는 경우에는 기본 충돌체로 사용됨
    std::weak_ptr<class CColliderSphere2D> mHitBox; //기본으로는 생성하지 않음, 필요한 경우에만 상속받은 객체에서 생성
    std::weak_ptr<class CSoundComponent> mSoundPlayer;

    std::weak_ptr<class CTearShooter> mShooter; //기본으로는 생성하지않음, 필요한 경우에만 상속받은 객체에서 생성

    //여러가지 용도로 사용가능한 오버랩 보관
    std::map<int, std::weak_ptr<class CCollider>> mOverlaps;

    //머리 몸이 바라보는 방향
    FVector2 mBodyDirection;
    FVector2 mHeadDirection;

    FUnitAttribute mAttribute; //능력치
    FUnitAttribute mMagnification; // 능력치 배율
    std::vector<std::pair<EUnitEffect, float>> mStatusQueue; //상태와 유지시간

    bool mbIsDead = false;

    std::vector<std::weak_ptr<class CSound>> mHurtSound;


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

    virtual void GetHit(std::weak_ptr<CGameObject> From) = 0;

    virtual void Reset(bool HardReset = false) = 0;
protected:
    bool AddAnim(const std::string& Name, const TCHAR* FilePath, bool Upper = false, float PlayTime = 1.f, float PlayRate = 1.f, bool Loop = false, bool Reverse = false, bool Symmetry = false);
    virtual void Move(const FVector3& Force) const;

    virtual void Fire();
    //죽었을때 죽고 나서 애니메이션이나 효과를 출력해야하는 경우가 많기 때문에 즉시 제거하지 않고 시간을 둠
    //전부 마치고 앞으로도 아무것도 하지 않는 상태가 되더라도 제거하지 않고 두기
    //상황에 따라서 몬스터가 살아있는 채로 맵을 리셋해야하는 경우가 있는데 
    //이런경우에 다시 생성하는 것 보다 이미 있는 객체를 활성화 하는 방식이 낮다고 생각함
    virtual void Dead();
                        
    virtual void OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider) = 0;
    virtual void ExitHurtOverlaps(std::weak_ptr<CCollider> Collider) = 0;

    virtual void OnHitOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider) = 0;
    virtual void ExitHitOverlaps(std::weak_ptr<CCollider> Collider) = 0;

    virtual void SetBodyDirection(FVector2 Dir);
    virtual void SetHeadDirection(FVector2 Dir);

    virtual void PlayBodyVerticalAnim();
    virtual void PlayBodyHorizontalAnim();
    virtual void PlayHeadVerticalAnim();
    virtual void PlayHeadHorizontalAnim();

public:
    const bool GetIsDead() const { return mbIsDead; }

    FVector2 GetBodyDirection() const { return mBodyDirection; }
    FVector2 GetHeadDirection() const { return mHeadDirection; }

    std::weak_ptr<CSceneComponent> GetHeadComp();
    std::weak_ptr<CSceneComponent> GetBodyComp();
    std::weak_ptr<class CTearShooter> GetShooterComp() { return mShooter; }

    FUnitAttribute GetAttribute() { return mAttribute; }
    void AddAttribute(FUnitAttribute attribute) { mAttribute += attribute; }
    void SetAttribute(FUnitAttribute attribute) { mAttribute = attribute; }
    FUnitAttribute GetMagnification() { return mMagnification; }
    void AddMagnification(FUnitAttribute magnification) { mMagnification += magnification; }
    void SetMagnification(FUnitAttribute magnification) { mMagnification = magnification; }
};                                              

