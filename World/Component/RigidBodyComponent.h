#pragma once
#include "World\SceneComponent.h"

class CRigidBodyComponent :
    public CSceneComponent
{
public:
    CRigidBodyComponent();
    CRigidBodyComponent(const CRigidBodyComponent& src);
    CRigidBodyComponent(CRigidBodyComponent&& src) noexcept;
    virtual ~CRigidBodyComponent();

private:
    FVector3 mRBVelocity;
    float mRBSpeedLimit;
    float mMass = 5.f;
    float mGravity = 9.8f;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Destroy();

public:
    FVector3 GetVelocity() { return mRBVelocity; }
    float GetForce() { return mRBVelocity.Length(); }

    void AddForce(FVector3 Force) { mRBVelocity += Force; }
    void SetVelocity(FVector3 Velocity) { mRBVelocity = Velocity; }
    void SetLimit(float Limit) { mRBSpeedLimit = Limit; }
    void SetMass(float Mass) { mMass = Mass; }
    void SetGravity(float Gravity) { mGravity = Gravity; }
};

