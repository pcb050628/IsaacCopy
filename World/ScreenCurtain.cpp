#include "ScreenCurtain.h"

#include "Device.h"
#include "TimeManager.h"
#include "Component/SpriteComponent.h"

CScreenCurtain::CScreenCurtain()
{
}

CScreenCurtain::~CScreenCurtain()
{
}

bool CScreenCurtain::Init()
{
    mBackground = CreateComponent<CSpriteComponent>("Root");
    mGround = CreateComponent<CSpriteComponent>("Ground");
    mCharacter = CreateComponent<CSpriteComponent>("Character");
    mLight = CreateComponent<CSpriteComponent>("Light");
    if (mBackground.expired() || mGround.expired() || mCharacter.expired() || mLight.expired())
        return false;

    std::shared_ptr<CSpriteComponent> bg = mBackground.lock();
    std::shared_ptr<CSpriteComponent> gd = mGround.lock();
    std::shared_ptr<CSpriteComponent> ch = mCharacter.lock();
    std::shared_ptr<CSpriteComponent> lt = mLight.lock();

    bg->SetMesh("TexRect"); bg->SetShader("Sprite2D"); bg->SetRenderLayer(11);
    gd->SetMesh("TexRect"); gd->SetShader("Sprite2D"); gd->SetRenderLayer(12);
    ch->SetMesh("TexRect"); ch->SetShader("Sprite2D"); ch->SetRenderLayer(13);
    lt->SetMesh("TexRect"); lt->SetShader("Sprite2D"); lt->SetRenderLayer(14);

    bg->SetSpriteData("UI_Nightmare_Sequence_Background");
    gd->SetSpriteData("UI_Nightmare_Sequence_Ground");
    ch->SetSpriteData("UI_Nightmare_Sequence_Isaac");
    lt->SetSpriteData("UI_Nightmare_Sequence_Light");

    FResolution resol = CDevice::GetInst()->GetResolution();

    bg->SetWorldScale(resol.Width, resol.Height);

    gd->SetRelativeScale(0.4f, 0.4f);
    ch->SetRelativeScale(0.2f, 0.2f);
    lt->SetRelativeScale(0.8f, 0.8f);

    gd->SetRelativePos(0.f, -350.f);
    ch->SetRelativePos(0.f, -150.f);

    return true;
}

void CScreenCurtain::Update(float DeltaTime)
{
    //화면 가리기
    //mRemainingTime 만큼 지난 후 1초에 걸쳐 알파 낮추기
    //1초가 지나면 객체 비활성화 하기
    if (mbStartFade)
    {
        float val = 1.f - mTimeStack;
        mBackground.lock()->SetOpacity(0, val);
        mGround.lock()->SetOpacity(0, val);
        mCharacter.lock()->SetOpacity(0, val);
        mLight.lock()->SetOpacity(0, val);
        mTimeStack += DeltaTime;
    }

    CActor::Update(DeltaTime);
}

void CScreenCurtain::Start()
{
    mbStartFade = false;

    SetEnable(true);
    SetRenderEnable(true);

    mBackground.lock()->SetOpacity(0, 1);
    mGround.lock()->SetOpacity(0, 1);
    mCharacter.lock()->SetOpacity(0, 1);
    mLight.lock()->SetOpacity(0, 1);

    CTimeManager::SetTimer(mDuration, false, this, &CScreenCurtain::StartFade);
}

void CScreenCurtain::StartFade()
{
    mbStartFade = true;
    CTimeManager::SetTimer(1.f, false, this, &CScreenCurtain::EndFade);
}

void CScreenCurtain::EndFade()
{
    SetEnable(false);
    SetRenderEnable(false);
    mOnFadeOut();
}
