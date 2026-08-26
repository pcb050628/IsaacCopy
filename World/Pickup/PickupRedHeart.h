#pragma once
#include "../Base/Pickup.h"

class CPickupRedHeart :
    public CPickup
{
    GAMEOBJCLASS(CPickupRedHeart, 81)
public:
    CPickupRedHeart();
    virtual ~CPickupRedHeart();

public:
    virtual bool Init() override;
};

