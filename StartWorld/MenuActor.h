#pragma once
#include "ScreenActor.h"

class CMenuActor :
    public CScreenActor
{
public:
    CMenuActor();
    virtual ~CMenuActor();

private:
    std::weak_ptr<class CSpriteComponent> mBackground;
    std::weak_ptr<class CSpriteComponent> mElementRun;
    std::weak_ptr<class CSpriteComponent> mElementContinue;
    std::weak_ptr<class CSpriteComponent> mElementPointer;
    int mFocused = 0;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    void UpdatePointerPos();

public:
    virtual int OnUp() override;
    virtual int OnDown() override;
    virtual int OnRight() override;
    virtual int OnLeft() override;
    virtual int OnSubmit() override;
    virtual int OnEscape() override;

};

