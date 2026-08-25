#include "GameObject.h"

#include "../Chapter.h"

CGameObject::CGameObject(EObjectType Type)
	:CGameClass(Type)
{
}

CGameObject::CGameObject(const CGameObject& src)
	:CGameClass(src)
{
}

CGameObject::CGameObject(CGameObject&& src) noexcept
	:CGameClass(std::move(src))
{
}

CGameObject::~CGameObject()
{
}

bool CGameObject::Init()
{
	if (mWorld.expired())
		return false;
	mChapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (mChapter.expired())
		return false;

	return true;
}

void CGameObject::ReturnToChapter()
{
	auto chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	chptr->ReturnGObj(GetThisPtr<CGameObject>());
}
