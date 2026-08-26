#pragma once
#include "../Base/Item.h"

class CItemBelt :
    public CItem
{
    GAMEDEFCLASS(CItemBelt, 91)
public:
    CItemBelt();
    virtual ~CItemBelt();

public:
    virtual bool Init(const std::weak_ptr<CGameClass>& Owner) override;
    virtual void Reset(bool HardReset = false);

public:
    virtual void OnGetItem(const std::weak_ptr<class CCharacter>& character) override;
};

