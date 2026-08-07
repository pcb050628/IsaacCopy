#pragma once
#include "../Base/Character.h"

class CCharacterIsaac :
    public CCharacter
{
    GAMECLASS(CCharacterIsaac, 31)
public:
    CCharacterIsaac();
    virtual ~CCharacterIsaac();

protected:


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

protected:
    virtual void PlayBodyVerticalAnim();
    virtual void PlayBodyHorizontalAnim();
    virtual void PlayHeadVerticalAnim();
    virtual void PlayHeadHorizontalAnim();

private:


};

