#pragma once
#include "World\ActorComponent.h"
#include "../Base/GameObject.h"
#include "../UnitData.h"

class CTearShooter :
    public CActorComponent
{
public:
    CTearShooter();
    virtual ~CTearShooter();

protected:
    std::weak_ptr<class CUnitbase> mOwnerUnit;
    std::weak_ptr<class CSceneComponent> mOwnerUnitHead;
    std::weak_ptr<class CSceneComponent> mOwnerUnitBody;
    bool mbIsOwnerPlayer = false;
    //발사 위치들 | 당연히 offset 개념으로 작동함
    std::vector<FVector2> mFirePoints;
    int mFirePointIndex = 0;
    //발사를 순서대로 할건지 동시에 하는지
    bool mbIsSynchronized = false;
    //마지막 발사 시간
    float mLastFireTime = 0.f;

    FUnitAttribute mUnitAttribute;
    FTearAttribute mTearAttribute; //눈물 설정

    //눈물 충돌시 호출될 함수들
    std::unordered_map<int, std::function<void(const FVector3&, const FVector3&, std::weak_ptr<class CCollider>)>> mOnCollisionFunc;
    //눈물 파괴시 호출될 함수들
    std::unordered_map<int, std::function<void(const FVector3&)>> mOnDestroyFunc;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

public:
    void Fire();
    //IsSet - True: 위치 고정 | False: 오프셋 추가
    void Fire(FVector3 firePoint, bool IsSet = false);
    void FireWithVelocityOffset(FVector2 vOffset);

    void UpdateUnitAttributeData(const bool Synchronize, FUnitAttribute Attribute);
    void UpdateTearAttributeData(FTearAttribute Attribute);
    void UpdateAttributeData(FUnitAttribute Unit, FTearAttribute Tear);
    //싱크로나이즈 설정
    void SetSynchro(const bool Val) { mbIsSynchronized = Val; }
    //발사 위치 등록 | 항상 아래쪽을 기준으로 추가하기
    void AddFirePoint(const FVector2& Point);
    void RemoveFirePoint(const FVector2& Point);
    //충돌시 등록
    template<typename T>
    void AddOnCollision(T* Object, void(T::* Func)(const FVector3&, const FVector3&, std::weak_ptr<CCollider>))
    {
        CGameObject* obj = Object;
        int id = obj->GetID();
        mOnCollisionFunc.insert(std::make_pair(id, std::bind(Func, Object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    }
    //파괴시 등록
    template<typename T>
    void AddOnDestroy(T* Object, void(T::* Func)(const FVector3&))
    {
        CGameObject* obj = static_cast<CGameObject>(Object);
        int id = obj->GetID();
        mOnDestroyFunc.insert(std::make_pair(id, std::bind(Func, Object, std::placeholders::_1)));
    }

    //id로 해놓은 이유는 할당하는 객체들이 전부 유닛이 아니라 유닛과 아이템들로 이루어지고 객체 하나당 하나만 할당하기 때문
    void RemoveOnCollision(int ID)
    {
        if (mOnCollisionFunc.find(ID) == mOnCollisionFunc.end())
            return;
        mOnCollisionFunc.erase(ID);
    }
    void RemoveOnDestroy(int ID)
    {
        if (mOnDestroyFunc.find(ID) == mOnDestroyFunc.end())
            return;
        mOnDestroyFunc.erase(ID);
    }

private:
    FVector3 FirePointCalculate(int Index);

public:
    //눈물에서 호출할 함수
    //찾아보니 function 의 할당 및 복사 비용이 weak랑 비교해서 너무 비쌈
    void OnDestroy(const FVector3& WorldPos);
    void OnCollision(const FVector3& Point, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);

public:
    void SetTearAttribute(FTearAttribute Attribute) { mTearAttribute = Attribute; }
    FTearAttribute GetTearAttribute() { return mTearAttribute; }

    const bool GetIsHoming() const { return mTearAttribute.Homing; }
    const bool GetIsPiercing() const { return mTearAttribute.Piercing; }
    const bool GetIsSpectral() const { return mTearAttribute.Spectral; }
    const bool GetIsOrbiting() const { return mTearAttribute.Orbiting; }
    const bool GetIsBoomerang() const { return mTearAttribute.Boomerang; }
    const bool GetIsShield() const { return mTearAttribute.Shield; }
    const bool GetIsBouncing() const { return mTearAttribute.Bouncing; }

    void SetHoming(const bool Val) { mTearAttribute.Homing = Val; }
    void SetPiercing(const bool Val) { mTearAttribute.Piercing = Val; }
    void SetSpectral(const bool Val) { mTearAttribute.Spectral = Val; }
    void SetOrbiting(const bool Val) { mTearAttribute.Orbiting = Val; }
    void SetBoomerang(const bool Val) { mTearAttribute.Boomerang = Val; }
    void SetShield(const bool Val) { mTearAttribute.Shield = Val; }
    void SetBouncing(const bool Val) { mTearAttribute.Bouncing = Val; }
};

