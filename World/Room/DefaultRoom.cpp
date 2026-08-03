#include "DefaultRoom.h"
#include "../Base/Unitbase.h"

#include "../Manager/GameClassContainer.h"

#include "World/WorldManager.h"
#include "../Chapter.h"

REGISTER_GAMECLASS(CDefaultRoom, "Room_Default", EObjectType::Room);

CDefaultRoom::CDefaultRoom()
	:CRoombase(ERoomType::Normal, ERoomShape::Normal)
{
	
}

CDefaultRoom::~CDefaultRoom()
{
}

bool CDefaultRoom::Init()
{
	if (!CRoombase::Init())
		return false;

	//메시에 텍스쳐 넣기

	return true;
}

void CDefaultRoom::Update(float DeltaTime)
{
	CRoombase::Update(DeltaTime);
}

void CDefaultRoom::Destroy()
{
	CActor::Destroy();
}

void CDefaultRoom::OnEnterRoom()
{
}

void CDefaultRoom::WinRoom()
{
	mbIsRoomWin = true;
	//보상 스폰하기
}

void CDefaultRoom::OnExitRoom()
{
}

bool CDefaultRoom::WinCheck()
{
	bool flag = false;
	for (std::weak_ptr<CUnitbase> unit : mUnits)
	{
		if (unit.lock()->IsEnable())
		{
			return false;
		}
	}
	return true;
}