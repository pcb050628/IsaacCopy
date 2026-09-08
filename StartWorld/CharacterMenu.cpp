#include "CharacterMenu.h"

#include "Device.h"
#include "../World/Component/SpriteComponent.h"

CCharacterMenu::CCharacterMenu()
{
}

CCharacterMenu::~CCharacterMenu()
{
}

bool CCharacterMenu::Init()
{
    //회전을 위해서 정해줘야하는 것들
    //1. 정면 위치
    //2. 후면 위치
    //3. 측면 위치
    //그 외의 계산방법
    //중간 값보다 작으면 오른쪽 크면 왼쪽
    //y축은 2분의 1길이에 비례하고 x축은 4분의 1길이에 비례함
    //y는 절반 지점까지 증가하다가 절반이 넘으면 다시 감소하고
    //x는 측면의 절반까지 증가하고 그 후 후면까지 감소
    //x는 전체길이의 절반만큼의 오프셋을 가진 루프이고
    //y는 전체길이의 루프
    //0, 0 | 최대길이 10이면
    mBackground = CreateComponent<CSpriteComponent>("Root");
    mFrame = CreateComponent<CSpriteComponent>("Frame");
    mBackground.lock()->SetMesh("TexRect"); mBackground.lock()->SetShader("Sprite2D"); mBackground.lock()->SetRenderLayer(1);
    mFrame.lock()->SetMesh("TexRect"); mFrame.lock()->SetShader("Sprite2D"); mFrame.lock()->SetRenderLayer(1);
    FResolution resol = CDevice::GetInst()->GetResolution();
    mBackground.lock()->SetWorldScale(resol.Width, resol.Height);
    mFrame.lock()->SetWorldScale(resol.Width, resol.Height);
    mBackground.lock()->SetSpriteData("UI_Game_Character_Menu_Background");
    mFrame.lock()->SetSpriteData("UI_Game_Character_Menu_Frame");
    for (int i = 0; i < 8; ++i)
    {
        mSprites.push_back(CreateComponent<CSpriteComponent>("Character"));
		mSprites[i].lock()->SetMesh("TexRect"); mSprites[i].lock()->SetShader("Sprite2D"); mSprites[i].lock()->SetRenderLayer(2);
        mSprites[i].lock()->SetWorldScale(100.f, 100.f);
        std::string str = i % 2 == 0 ? "Spoon_Bender" : "Crooked_Penny";
        mSprites[i].lock()->SetSpriteData(str);
    }

    mWidth = 600.f;
    mHeight = 200.f;

    mCenterOffset.x = -mWidth / 3 + -40;
    mCenterOffset.y = 70.f;

    ResettingPos();
    return true;
}

void CCharacterMenu::Update(float DeltaTime)
{
    CActor::Update(DeltaTime);
}

void CCharacterMenu::MoveRight()
{
    mFocusedIndex = (mFocusedIndex + 1) % mSprites.size();
    ResettingPos();
}

void CCharacterMenu::MoveLeft()
{
    mFocusedIndex = (mFocusedIndex - 1) % mSprites.size();
    ResettingPos();
}

void CCharacterMenu::ResettingPos()
{
    //x는 왼쪽끝부터 오른쪽끝까지의 거리 / (10 / 2) * (인덱스+5)
    //y는 정면부터 후면까지의 거리 / (10/2) * 인덱스
    int length = mSprites.size();
    int xo = length / 2 - 1;
    int yo = 0;
    bool xc = true;
    bool yc = true;
    for (size_t i = 0; i < length; i++)
    {
        float x = mWidth / length * xo;
        float y = mHeight / length * yo;
        mSprites[(i + mFocusedIndex) % length].lock()->SetRelativePos(mCenterOffset + FVector2(x, y));
        //스케일은 y에 비례해서 정해주기
        mSprites[(i + mFocusedIndex) % length].lock()->SetWorldScale((length - yo) * 10.f, (length - yo) * 10.f);

        if (xc && xo >= length - length / 4 - 1)
            xc = false;
        else if (!xc && xo < length / 4)
            xc = true;

        if (yc && yo >= length / 2)
            yc = false;
        else if (!yc && yo <= 0)
            yc = true;

        xo = xc ? xo + 1 : xo - 1;
        yo = yc ? yo + 1 : yo - 1;
    }
}

int CCharacterMenu::OnUp()
{
    return 0;
}

int CCharacterMenu::OnDown()
{
    return 0;
}

int CCharacterMenu::OnRight()
{
    MoveRight();
    return 0;
}

int CCharacterMenu::OnLeft()
{
    MoveLeft();
    return 0;
}

int CCharacterMenu::OnSubmit()
{
    return 1;
}

int CCharacterMenu::OnEscape()
{
    return 1;
}
