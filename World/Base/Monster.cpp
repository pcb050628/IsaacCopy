#include "Monster.h"

CMonster::CMonster()
	:CUnitbase(EObjectType::Monster)
{
}

CMonster::CMonster(const CMonster& src)
	:CUnitbase(src)
{
}

CMonster::CMonster(CMonster&& src) noexcept
	:CUnitbase(std::move(src))
{
}

CMonster::~CMonster()
{
}
