#include "GameObject.h"

#include "../Chapter.h"

int CGameObject::GlobalID = 0;

CGameObject::CGameObject(EObjectType Type)
	:mID(GlobalID++), mObjType(Type)
{
}

CGameObject::CGameObject(const CGameObject& src)
	:CActor(src), mID(GlobalID++), mObjType(src.mObjType)
{
}

CGameObject::CGameObject(CGameObject&& src) noexcept
	:CActor(std::move(src)), mID(src.mID), mObjType(src.mObjType)
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::ReturnToChapter()
{
	auto chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	chptr->ReturnGObj(GetThisPtr<CGameObject>());
}
