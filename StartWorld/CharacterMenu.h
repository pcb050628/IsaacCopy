#pragma once
#include "ScreenActor.h"

class CCharacterMenu :
    public CScreenActor
{
public:
    CCharacterMenu();
    virtual ~CCharacterMenu();

private:
    std::weak_ptr<class CSpriteComponent> mBackground;
    std::weak_ptr<class CSpriteComponent> mFrame;
    std::vector<std::weak_ptr<class CSpriteComponent>> mSprites;
    int mFocusedIndex = 0;
    FVector2 mCenterOffset = FVector2::Zero;
    int mWidth = 0;
    int mHeight = 0;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    void MoveRight();
    void MoveLeft();

    void ResettingPos();

public:
    virtual int OnUp() override;
    virtual int OnDown() override;
    virtual int OnRight() override;
    virtual int OnLeft() override;
    virtual int OnSubmit() override;
    virtual int OnEscape() override;
};

