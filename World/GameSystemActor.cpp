#include "GameSystemActor.h"
#include "World/CameraComponent.h"

CChapterSystemActor::CChapterSystemActor()
{
}

CChapterSystemActor::~CChapterSystemActor()
{
}

bool CChapterSystemActor::Init()
{
    mCam = CreateComponent<CCameraComponent>("Root");
    if (mCam.expired())
        return false;

    return true;
}

void CChapterSystemActor::Update(float DeltaTime)
{
}
