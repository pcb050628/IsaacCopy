#pragma once
#include "EssentialHeader.h"
#include "../../GameObjectEnums.h"

class CGameClass
{
public:
    virtual const int GetGClassID() = 0;
public:
    CGameClass(EObjectType Type);
    CGameClass(const CGameClass& src);
    CGameClass(CGameClass&& src) noexcept;
    virtual ~CGameClass() = 0;

protected:
    const EObjectType mObjType;
    const int mID;

public:
    const int GetID() const { return mID; }
    const EObjectType GetObjType() const { return mObjType; }

private:
    static int GlobalID;

};

