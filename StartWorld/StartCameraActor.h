#pragma once
#include "World\Actor.h"

class CStartCameraActor :
    public CActor
{
public:
    CStartCameraActor();
    virtual ~CStartCameraActor();

private:
    std::weak_ptr<class CRigidBodyComponent> mRb;
    std::weak_ptr<class CCameraComponent> mCam;
    std::function<void()> mOnMoveEnd;
    float mTargetHeight = -1.f;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    void MoveUp();
    void MoveDown();

    template<typename T>
    void SetMoveEndFunction(T* Obj, void(T::* func)())
    {
        mOnMoveEnd = std::bind(func, Obj);
    }
};

