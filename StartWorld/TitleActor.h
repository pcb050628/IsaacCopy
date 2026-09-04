#pragma once
#include "C:\Users\assort_c5\source\repos\DirectX_Source\Engine\Include\World\Actor.h"

class CTitleActor :
    public CActor
{
public:
    CTitleActor();
    virtual ~CTitleActor();

private:
    std::weak_ptr<class CMeshComponent> mMeshComponent;
    std::weak_ptr<class CAnimation2DComponent> mAnimator;
    std::weak_ptr<class CSpriteComponent> mBackground;
    std::weak_ptr<class CRigidBodyComponent> mTitleRB;
    std::weak_ptr<class CSpriteComponent> mTitleSprite;
    float mTitlePivotPointY = 0;
    float mTitleMoveDistMax = 15.f;
    bool mbMoveUp = true;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destory();

};

