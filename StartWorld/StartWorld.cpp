#include "StartWorld.h"
#include "TitleActor.h"

#include "World/CameraComponent.h"

CStartWorld::CStartWorld()
{
}

CStartWorld::~CStartWorld()
{
}

bool CStartWorld::Init()
{
    if (!CWorld::Init())
        return false;
    if (CreateActor<CTitleActor>("Title").expired())
        return false;
    std::shared_ptr<CActor> cam = CreateActor<CActor>("cam").lock();
    std::shared_ptr<CCameraComponent> camComp = cam->CreateComponent<CCameraComponent>("Root").lock();
    return true;
}

void CStartWorld::Update(float DeltaTime)
{
    CWorld::Update(DeltaTime);
}
