#include "RoomMember.h"

CRoomMember::CRoomMember(EObjectType Type)
	:CGameObject(Type)
{
}

CRoomMember::CRoomMember(const CRoomMember& src)
	:CGameObject(src)
{
}

CRoomMember::CRoomMember(CRoomMember&& src) noexcept
	:CGameObject(std::move(src))
{
}

CRoomMember::~CRoomMember()
{
}
