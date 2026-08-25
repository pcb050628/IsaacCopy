#pragma once
#include "../base/Item.h"

class CItemSpoonBender :
    public CItem
{
    GAMEDEFCLASS(CItemSpoonBender, 92)
public:
    CItemSpoonBender();
    virtual ~CItemSpoonBender();

public:
    virtual bool Init() override;
    virtual void Destroy() override;
    virtual void Reset(bool HardReset = false);

public:
    virtual void OnGetItem(const std::weak_ptr<class CCharacter>& character) override;
};

