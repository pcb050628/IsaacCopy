#include "GameRuleManager.h"

#include "LogManager.h"

#include "../Base/Roombase.h"

CGameRuleManager* CGameRuleManager::mInstance = nullptr;

CGameRuleManager::CGameRuleManager()
{
}
CGameRuleManager::~CGameRuleManager()
{

}

bool CGameRuleManager::Init()
{

	return false;
}

void CGameRuleManager::SetRandomSeed(unsigned int Seed)
{
	if (1 == Seed)
		Seed = mRd();

	mGen.seed(Seed);
	mfDistribution = std::uniform_real_distribution<float>(0);
	LOG_DEBUG("seed: ", Seed);
}