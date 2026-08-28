#pragma once
#include "World\Actor.h"
#include "UnitData.h"

//방에 종속되지않는 객체
//카메라 및 UI 역할을 겸함 / 각각의 역할을 하는 액터 컴포넌트를 각각 지님 각각각각각각각각
//포커스된 방을 비추고
//UI On/Off 시 움직이는 효과등은 이 객체에 있음
class CChapterSystemActor :
    public CActor
{
public:
    CChapterSystemActor();
    virtual ~CChapterSystemActor();

protected:
    std::weak_ptr<class CCameraComponent> mCam;
    std::weak_ptr<class CSoundComponent> mSound;
    std::weak_ptr<class CRigidBodyComponent> mRb;
    //UI도 여기 넣을까 말까
    //넣을 생각이긴 헀는데
    //일단 넣고 커지면 분리할까?
    //좀 커도 그냥 넣어놓ㅇ르까
    FVector3 mHeartSpriteOffset = FVector3::Zero;
    float mHeartSpriteSize = 50.f;
    float mHeartSpriteSpacing = 50.f;
    float mHeartSpritePadding = 10.f;
    std::vector<std::weak_ptr<class CSpriteComponent>> mHearts; // 최대 12

    FVector3 mTargetPosition = -FVector3::One;

    bool mbIsMoving = false;
    float mCompareDist = 10.f;

    std::function<void()> mMoveEndFunc;

public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;

private:
    void MoveUp();
    void MoveDown();
    void MoveRight();
    void MoveLeft();

    void MoveToTarget(float DeltaTime);

    void UpdateHeart(int id, FPlayerHeartContainer container);

public:
    void Move(FVector2 dir);


public:
    template<typename T>
    void OnMoveEnd(T* Object, void (T::* Func)())
    {
        mMoveEndFunc = std::bind(Func, Object);
    }

};

