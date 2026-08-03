#pragma once
#include "World\Actor.h"

enum class EDoorOpenRequirement
{
    None,   //클리어시 자동으로 열림
    Key,    //열쇠로 열림
    Key2,   //열쇠 두개로 열림
    Blast,  //폭파로 열림
    Blast2, //폭파 두번으로 열림
};

class CDoor :
    public CActor
{
public:
    CDoor();
    CDoor(const CDoor& src);
    CDoor(CDoor&& src) noexcept;
    virtual ~CDoor();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

protected:
    std::weak_ptr<class CRoombase> mRoomOwner;
    std::weak_ptr<class CColliderBox2D> mBoxColComp;
    std::weak_ptr<class CMeshComponent> mMeshComp; //애니메이터 작성시 변경하기

    bool mbIsOpen = true;
    EDoorOpenRequirement mRequirement = EDoorOpenRequirement::None;

    std::weak_ptr<class CUnitbase> mPlayer;

public:
    void SetOpen(bool Val);
    //다른 방 이동 함수 작성 및 연결하기

private:
    void OnOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    void ExitOverlaps(std::weak_ptr<class CCollider> Collider);
};

