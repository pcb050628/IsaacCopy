#include "GameDefinition.h"

CGameDefinition::CGameDefinition(EObjectType Type)
	:CGameClass(Type)
{
}

CGameDefinition::CGameDefinition(const CGameDefinition& src)
	:CGameClass(src)
{
}

CGameDefinition::CGameDefinition(CGameDefinition&& src) noexcept
	:CGameClass(std::move(src))
{
}

CGameDefinition::~CGameDefinition()
{
}

bool CGameDefinition::Init(const std::weak_ptr<CGameClass>& Owner)
{
	if (Owner.expired())
		return false;

	mOwner = Owner;
	return true;
}
