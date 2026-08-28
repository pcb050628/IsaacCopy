#pragma once
#include "../Base/Pickup.h"

class CPickupRedHeart :
    public CPickup
{
    GAMEOBJCLASS(CPickupRedHeart, 81)
public:
    CPickupRedHeart();
    virtual ~CPickupRedHeart();

protected:
    EPlayerHeartState mHeartState = EPlayerHeartState::Full;

public:
    virtual bool Init() override;

protected:
    virtual bool TryGet(std::weak_ptr<class CCharacter> chara) override;
};

