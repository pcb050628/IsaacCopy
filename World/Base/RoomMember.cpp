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

void CRoomMember::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	assert(!mRoomOwner.expired() && "객체가 등록되지 않은 채로 실행중입니다.");
}
