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

    float mIntensity = 0.f;
    float mRemainTime = 5.f;
    int mCurrentPaletteIndex = 0;

    static FVector4 ColorPalette[4];

public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void Reset(bool HardReset = false);

    void Ignite();
    void Explosion();

private:
    void OnAreaOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    void ExitAreaOverlaps(std::weak_ptr<class CCollider> Collider);

protected:
    virtual bool TryGet(std::weak_ptr<class CCharacter> chara) override;
};

