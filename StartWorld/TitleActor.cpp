#include "TitleActor.h"

#include "Device.h"

#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"
#include "../World/Component/RigidBodyComponent.h"
#include "../World/Component/SpriteComponent.h"

CTitleActor::CTitleActor()
{
}

CTitleActor::~CTitleActor()
{
}

bool CTitleActor::Init()
{
    mBackground = CreateComponent<CSpriteComponent>("Root");
    mTitleRB = CreateComponent<CRigidBodyComponent>("TitleBody");
    mTitleSprite = CreateComponent<CSpriteComponent>("TitleSprite", "TitleBody");
    mMeshComponent = CreateComponent<CMeshComponent>("Mesh");
    mAnimator = CreateComponent<CAnimation2DComponent>("Animator");;

    if (mBackground.expired() || mTitleSprite.expired() || mMeshComponent.expired() || mAnimator.expired())
        return false;

    std::shared_ptr<CSpriteComponent> bg = mBackground.lock();
    std::shared_ptr<CSpriteComponent> title = mTitleSprite.lock();
    std::shared_ptr<CMeshComponent> mesh = mMeshComponent.lock();
    std::shared_ptr<CAnimation2DComponent> animator = mAnimator.lock();

    bg->SetMesh("TexRect"); bg->SetShader("Sprite2D"); bg->SetRenderLayer(0);
    title->SetMesh("TexRect"); title->SetShader("Sprite2D"); title->SetRenderLayer(1);
    mesh->SetMesh("TexRect"); mesh->SetShader("Animation2D"); mesh->SetRenderLayer(2);

    FResolution resol = CDevice::GetInst()->GetResolution();
    bg->SetSpriteData("UI_Game_Title_Background");
    bg->SetWorldScale(resol.Width, resol.Height);

    title->SetSpriteData("UI_Game_Title");
    mTitleRB.lock()->SetRelativeScale(1, 0.3f);
    mTitleRB.lock()->SetRelativePos(0, 240.f);
    mTitleRB.lock()->SetLimit(300.f);

    mesh->SetRelativeScale(0.3f, 0.5f);
    mesh->SetRelativePos(0.f, -50.f); mesh->SetRenderState(0, "AlphaBlend");
    animator->SetUpdateComponent(mMeshComponent);// animator->SetUpdateEnable(true);
    animator->AddAnimation("UI_Game_Title_Animation", 0.8f, 1.f, true);

    mTitlePivotPointY = mTitleRB.lock()->GetRelativePos().y;
    return true;
}

void CTitleActor::Update(float DeltaTime)
{
    std::shared_ptr<CRigidBodyComponent> title = mTitleRB.lock();
    if (mbMoveUp)
    {
        title->AddForce(FVector3(0, 1, 0) * 300.f * DeltaTime);
    }
    else
    {
        title->AddForce(FVector3(0, -1, 0) * 300.f * DeltaTime);
    }

    float y = title->GetRelativePos().y;
    if (fabs(y - mTitlePivotPointY) > mTitleMoveDistMax)
    {
        if(y > mTitlePivotPointY && mbMoveUp)
            mbMoveUp = !mbMoveUp;
        else if (y < mTitlePivotPointY && !mbMoveUp)
            mbMoveUp = !mbMoveUp;
    }

    CActor::Update(DeltaTime);
}

void CTitleActor::Destory()
{
    CActor::Destroy();
}
