#include "GameClass.h"

int CGameClass::GlobalID = 0;

CGameClass::CGameClass(EObjectType Type)
	:mObjType(Type), mID(GlobalID++)
{
}

CGameClass::CGameClass(const CGameClass& src)
	:mObjType(src.mObjType), mID(GlobalID++)
{
}

CGameClass::CGameClass(CGameClass&& src) noexcept
	:mObjType(src.mObjType), mID(GlobalID++)
{
}

CGameClass::~CGameClass()
{
}
