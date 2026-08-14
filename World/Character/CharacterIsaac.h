#pragma once
#include "../Base/Character.h"

class CCharacterIsaac :
    public CCharacter
{
    GAMEOBJCLASS(CCharacterIsaac, 31)
public:
    CCharacterIsaac();
    virtual ~CCharacterIsaac();

protected:


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

private:
    void TestFuncForItemContainer();


};

