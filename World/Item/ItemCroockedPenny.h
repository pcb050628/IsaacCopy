#pragma once
#include "../Base/Item.h"

class CItemCroockedPenny :
    public CItem
{
    GAMEDEFCLASS(CItemCroockedPenny, 103)
public:
    CItemCroockedPenny();
    virtual ~CItemCroockedPenny();

public:
    virtual bool Init(const std::weak_ptr<CGameClass>& Owner) override;
    virtual void Reset(bool HardReset = false);

public:
    virtual bool UseItem();
    virtual void OnGetItem(const std::weak_ptr<class CCharacter>& character);
};

