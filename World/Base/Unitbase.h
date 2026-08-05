#pragma once
#include "GameObject.h"

#include "../UnitData.h"

class CUnitbase :
    public CGameObject 
{
public:
    CUnitbase(EObjectType Type);
    CUnitbase(const CUnitbase& src);
    CUnitbase(CUnitbase&& src) noexcept;
    virtual ~CUnitbase();
    
protected:
    std::weak_ptr<class CRoombase> mRoomOwner;
    std::weak_ptr<class CRigidBodyComponent> mRigidBody; //root
    std::weak_ptr<class CMeshComponent> mHeadMesh;
    std::weak_ptr<class CAnimation2DComponent> mHead;
    std::weak_ptr<class CMeshComponent> mBodyMesh;
    std::weak_ptr<class CAnimation2DComponent> mBody;
    std::weak_ptr<class CColliderSphere2D> mHurtBox;

    FUnitAttribute mAttribute;
    std::vector<std::pair<EUnitStatus, float>> mStatusQueue;

    bool mbIsTemporary = false;
    bool mbIsDead = false;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    //virtual void UnitUpdate(float DeltaTime, float GameTimeScale) = 0;
    virtual void Destroy();

    void SetRoom(std::weak_ptr<CRoombase> Room) { mRoomOwner = Room; }
    void UnsetRoom() { mRoomOwner.reset(); }

    virtual void GetHit(std::weak_ptr<CUnitbase> From) = 0;

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

public:
    const bool GetIsDead() const { return mbIsDead; }
    const bool GetIsTemporary() const { return mbIsTemporary; }
};

