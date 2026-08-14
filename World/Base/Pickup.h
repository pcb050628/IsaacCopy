#pragma once
#include "GameObject.h"
#include "../UnitData.h"

class CPickup :
    public CGameObject
{
public:
    CPickup(EPickupType Type, bool IsAboutCharacter = false, bool IsAboutPickup = false, int AffectPickupCount = 1, bool IsAboutInstantiate = false);
    virtual ~CPickup();

protected:
    //필요한 컴포넌트
    //1. 렌더해줘야지 -> 애니메이션 또는 스프라이트 | 그래서 Pickup에서 init 안해주고 상속받은 객체에서 하기
    //2. 유닛들에게 밀려야하기 때문에 -> 리지드바디
    //또 뭐가 있어야하나
    //충돌해야하는구나 콜라이더
    //끝?

    std::weak_ptr<class CRigidBodyComponent> mRigidbody;
    std::weak_ptr<class CColliderSphere2D> mCollider;

    std::weak_ptr<class CSpriteComponent> mSpriteRenderer;
    std::weak_ptr<class CMeshComponent> mMeshComp;
    std::weak_ptr<class CAnimation2DComponent> mAnimator;


    //픽업의 종류를 생각해보자
    //1. 하트(체력)
    //2. 열쇠
    //3. 동전
    //4. 폭탄
    //5. 자루(여러 종류의 픽업이 들어있는 픽업)
    //
    //사용하는 픽업
    //전부 모양과 효과가 정해져있음
    //룬은 
    //1. 룬 - 게임과 관련된 효과들
    //2. 카드 - 픽업또는 방과 관련된 효과들
    //3. 알약 - 캐릭터와 관련된 효과들

    //위 모든 픽업들의 공통점은
    //픽업은 오브젝트로 생성되고 획득시 효과만 있다는 것임
    //획득시 효과는 기본은 들고있는 것들의 개수를 추가하는 것이고
    //사용 픽업인 경우에는 픽업을 들고있다가 사용할 수 있다.
    //

    //능력치 관련 | 사칙연산 , 배율은 건드리지않음
    //체력관련 | 회복, 추가, 빼기, 피해
    //열쇠, 폭탄, 동전 관련 | 사칙연산, 값 스왑
    //사용관련
    //픽업 복사 (방 또는 들고있는 픽업들(열쇠, 동전, 폭탄))
    //픽업 소환
    //게임 룰 | 비밀방의 문을 상시 개방하기, 드랍률 및 드랍될 픽업의 상태 확률
    //플레이어 효과 | 호밍 효과, 능력치 상승, 무적 등
    //이동 | 다른 방으로 이동(랜덤 또는 지정)
    
    //그럼 다시 정리해서 함수로 만들어야 하는것
    //능력치 추가
    //열쇠,폭탄, 동전 추가
    //현재 방에 있는 픽업 복사? <- 이걸 어디로 옮기는게 맞을까 
    //픽업 생성 <- GameClassContainer의 Instantiate 하면 됨
    //플레이어 효과 <- 픽업은 플레이어 소유로 플레이어를 알아야 하기 때문에 문제없음 
    //              | 만약 모르게 설계해도 플레이어가 알아서 괜찮음 직접 대입이 아니라 픽업에서 리턴해주는 식으로 하면ㄷ ㅚㅁ
    //이동 -> 미루기 아직 관련된 기능을 안만듬 | 내용에 대해서는 별것 없을 수 있지만 지금 하기엔 뭔가 부담됨
    //게임 룰 관련 -> 나중으로 미루기 ( 아직 껍데기만 있고 내부는 정해진게 없는

    //능력치 추가
    //열쇠,폭탄,동전 추가
    //픽업 생성
    //픽업 복사

	const EPickupType mPickupType;
    //능력치 관련인지 | 픽업 (획득, 소실)관련인지 |
    const bool mbIsAboutCharacter;
    CharacterEffect::FPlayerEffect mEffect;
    
    //플레이어가 가지고 있는 픽업 갯수에 관여하는 경우
    const bool mbIsAboutPickup;
    EPickupType mAffectPickupType;
    const int mAffectPickupCount;
    
    //생성과 복사하는 픽업의 경우
    const bool mbIsAboutInstantiate;
    EPickupType mInstantiateTargetPickupType;

    //쓰고보니까 거지같은거 캐릭터에 능력치 변동 시 함수도 만들어야 되네
    //캐릭터에 추가해야 하는것
    //능력치 변동시 | 픽업 획득시 | 픽업 사용시 <- 픽업 사용시 가 있나? 아이템중에 ? 있네 시발 

public:
    virtual bool Init() override;
    virtual void Destroy() override;

protected:
    //충돌시
    //내부에서 처리해야하는 것
    //1. 플레이어 캐릭터가 아닌 유닛과는 충돌시 밀려 나야함
    //2. 플레이어인 경우에 함수 실행
    virtual void OnCollision(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider); 
    virtual void InstantiatePickup(); //복사와 생성 모두를 담당
    //함수로 안만들어도 되는것
    //1. 픽업 개수 관련 -> 어짜피 플레이어 내부 함수 호출해줘야해서 거기에서 개수도 조정해주면 됨
    //2. 능력치 관련도 마찬가지
    //그래서 만들어야 하는 함수는 생성하는 것 뿐임

};

