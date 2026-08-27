#pragma once
#include "Unitbase.h"

class CCharacter :
    public CUnitbase
{
public:
    CCharacter();
    virtual ~CCharacter() = 0;

protected:
    //유닛 외의 추가로 필요한것?
    //뭐가 있나?
    // 그나마 입력 세팅(게임 데이터)
    // 아 아이템 컨테이너 
    //그렇게 되면 현재 필요한건 없나?
    // rb, 머리(메시 애님), 몸(메시 애님), 히트박스, 허트박스, 능력치, 상태(이거 나중에 유닛 가서 만들어야 함)
    //

    std::weak_ptr<class CItemContainer> mItemContainer;
    FVector3 mMoveDirection;
    bool mbIsFiring = false;
    bool mbIsJustFired = false;

    std::string mHeadAnimName;
    std::string mBodyAnimName;

    std::string mDefaultHeadAnimName;
    std::string mDefaultBodyAnimName;

    std::weak_ptr<class CMeshComponent> mFullBodyMesh;
    std::weak_ptr<class CAnimation2DComponent> mFullBodyAnimator; //캐릭터 애니메이션 렌더 시 ( 점프, 순간이동 등 )
    bool mbIsFullBodyAnimPlaying = false;
    std::weak_ptr<class CSpriteComponent> mItemSprite; //캐릭터는 아닌데 캐릭터가 렌더해야하는 것 ( 아이템 획득 또는 사용시 )

public:
    virtual bool Init();//초기화 단계에서 첫번째 캐릭터인지 알아야함
    virtual void Update(float DeltaTime);
    virtual void Destroy();

    virtual void GetHit(std::weak_ptr<CGameObject> From);
    virtual void Reset(bool hard = false);

    virtual void OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    virtual void ExitHurtOverlaps(std::weak_ptr<CCollider> Collider);

    virtual void OnHitOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    virtual void ExitHitOverlaps(std::weak_ptr<CCollider> Collider);

public:
    //기능은 뭐가 필요한게 있나
    //초기화에서 키 입력 설정해주고
    //현재 입력가능한 상태인지는 챕터에서 관리할거고
    //결국 아이템 관련 뿐이 남는게 없는ㄷ
    //아이템 관련한것들을 제외하고는 뭐가 있니
    //아무리 생각해도 모르겠다.
    //아이템 추가 및 제거 함수
    //아이템 개수도 외부에서 알아야하고
    //아이템은 굳이 스마트 포인터로 들어야하나 | 포인터로 안들어도 될거같은데
    //아이템 만들때 가능하면 포인터로 안들고있어도 되게 만들자

    void SetDefaultHeadAnim() { mHeadAnimName = mDefaultHeadAnimName; }
    void OverrideHeadAnim(const std::string& Name);
    const std::string& GetHeadAnimName() const { return mHeadAnimName; }

    void SetDefaultBodyAnim() { mBodyAnimName = mDefaultBodyAnimName; }
    void OverrideBodyAnim(const std::string& Name);
    const std::string& GetBodyAnimName() const { return mBodyAnimName; }

public:
    void OnGetPickup(EPickupType Type, int Count);
    void OnLosePickup(EPickupType Type, int Count);
    void OnAttributeChanged();

    //나중에 만들어야 하는 것
    // 플레이어 이펙트라고 구조체는 만들어놨고
    // 내용은 어떤 효과를 얼마나 지속되게 줄건지임
    // 지속이라 맵이든 리스트는 배열이든 해서 들고 있다가 잘 확인하고 없애주면 됨
    //void AddPlayerEffect();

protected:
    virtual bool AddFullBodyAnim(const std::string& Name, const TCHAR* FilePath, float PlayTime, float PlayRate, bool Loop, bool Reverse, bool Symmetry);

    virtual void PlayFullBodyAnim(const std::string& Anim);

    virtual void PlayBodyVerticalAnim();
    virtual void PlayBodyHorizontalAnim();
    virtual void PlayHeadVerticalAnim();
    virtual void PlayHeadHorizontalAnim();

    virtual void PlayHeadAnim(bool Stop = false, bool Reset = false);
    virtual void PlayBodyAnim(bool Stop = false, bool Reset = false);

    virtual void RenderItemSprite(const std::string& SpriteName);

private:
    void MoveUp();
    void MoveLeft();
    void MoveDown();
    void MoveRight();

    virtual void Fire();
    void FireUp();
    void FireLeft();
    void FireDown();
    void FireRight();

    void UseItem();
    void UsePickup();
    void UseBomb();

    void DropPickupPress();
    void DropPickupRelease();

    void FullBodyAnimEnd();
    void RenderDisableItemSprite();

    void Attack(const FVector3& Point, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
};

