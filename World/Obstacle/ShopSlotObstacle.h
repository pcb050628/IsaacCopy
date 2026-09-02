#pragma once
#include "../Base/Obstaclebase.h"

class CShopSlotObstacle :
    public CObstaclebase
{
    GAMEOBJCLASS(CShopSlotObstacle, 402);
public:
    CShopSlotObstacle();
    virtual ~CShopSlotObstacle();

protected:
    int mItemID = -1;
    int mRequireCoin = 1;
    std::weak_ptr<class CSpriteComponent> mItemSpriteRenderer;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();
    virtual void Reset(bool Hard = false);

public:
    void SetItemID(const int ID);

private:
    void OnCollision(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
};

