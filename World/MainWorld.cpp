#include "MainWorld.h"

#include "World/CameraManager.h"
#include "World/CameraComponent.h"

CMainWorld::CMainWorld()
{}

CMainWorld::~CMainWorld()
{}

bool CMainWorld::Init()
{
	CWorld::Init();

	return true;
}

void CMainWorld::ChangeCamera()
{
	auto CameraMgr = GetCameraManager().lock();

	if (CameraMgr)
	{
		auto iter = mCameraList.begin();
		auto iterEnd = mCameraList.end();

		for (; iter != iterEnd;)
		{
			if (iter->expired())
			{
				iter = mCameraList.erase(iter);
				continue;
			}

			++iter;
		}

		if (mCameraIndex >= mCameraList.size())
		{
			mCameraIndex = 0;
		}

		CameraMgr->ChangeMainCamera(mCameraList[mCameraIndex++]);
	}
}
