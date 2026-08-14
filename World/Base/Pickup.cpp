#include "Pickup.h"

#include "World/ColliderSphere2D.h"
#include "../Component/RigidBodyComponent.h"

#include "../Base/Character.h"

CPickup::CPickup(EPickupType Type, bool IsAboutCharacter, bool IsAboutPickup, int AffectPickupCount, bool IsAboutInstantiate)
    :CGameObject(EObjectType::Pickup), mPickupType(Type), mEffect(0)
    ,mbIsAboutCharacter(IsAboutCharacter), mbIsAboutPickup(IsAboutPickup), mAffectPickupCount(AffectPickupCount), mbIsAboutInstantiate(IsAboutInstantiate)
{
}

CPickup::~CPickup()
{
}

bool CPickup::Init()
{
    mRigidbody = CreateComponent<CRigidBodyComponent>("Root");
    mCollider = CreateComponent<CColliderSphere2D>("Collider");
    if (mRigidbody.expired() || mCollider.expired())
        return false;

    std::shared_ptr<CRigidBodyComponent> rb = mRigidbody.lock();
    rb->SetLimit(300.f);
    rb->SetMass(1.f);
    rb->SetUseGravity(false);

    mCollider.lock()->SetCollisionProfile("PickUp");
    mCollider.lock()->SetBeginOverlapFunc(this, &CPickup::OnCollision);

    return true;
}

void CPickup::Destroy()
{
    CGameObject::Destroy();
}

void CPickup::OnCollision(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
    std::shared_ptr<CGameObject> obj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
    if (!obj)
        return;

    EObjectType t = obj->GetObjType();
    switch (t)
    {
    case EObjectType::PlayerCharacter:
        break;
    default:
    case EObjectType::Door:
    case EObjectType::Monster:
    case EObjectType::Obstacle:
    case EObjectType::Pickup:
    case EObjectType::End:
        mRigidbody.lock()->SetVelocity(-Normal * 100.f);
        return;
    case EObjectType::Tear:
        return;
    }

    if (EObjectType::PlayerCharacter == t)
    {
        std::shared_ptr<CCharacter> p = std::dynamic_pointer_cast<CCharacter>(obj);

        if (mbIsAboutPickup)
        {
            p->OnGetPickup(mPickupType, mAffectPickupCount);
        }
        else if (mbIsAboutCharacter)
        {

        }
        else if (mbIsAboutInstantiate)
        {

        }

        ReturnToChapter();
    }
}

void CPickup::InstantiatePickup()
{
}
