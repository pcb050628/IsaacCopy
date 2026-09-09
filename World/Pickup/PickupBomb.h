#pragma once
#include "../Base/Pickup.h"

class CPickupBomb :
    public CPickup
{
    GAMEOBJCLASS(CPickupBomb, 82)
public:
    CPickupBomb();
    virtual ~CPickupBomb();

protected:
    bool mbIsIgnited = false;
    std::weak_ptr<class CColliderSphere2D> mExplosionErea;

    std::map<int, std::weak_ptr<class CGameObject>> mInArea;

public:
    virtual bool Init() override;

    void Ignite();
    void Explosion();

protected:
    virtual bool TryGet(std::weak_ptr<class CCharacter> chara) override;
};

