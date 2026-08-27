#include "GameRuleManager.h"

#include "LogManager.h"

#include "../Base/Roombase.h"

#define HEART_MAX 12

CGameRuleManager* CGameRuleManager::mInstance = nullptr;

CGameRuleManager::CGameRuleManager()
{
}
CGameRuleManager::~CGameRuleManager()
{

}

bool CGameRuleManager::Init()
{


	return true;
}

void CGameRuleManager::SetRandomSeed(unsigned int Seed)
{
	if (1 == Seed)
		Seed = mRd();

	mGen.seed(Seed);
	mfDistribution = std::uniform_real_distribution<float>(0);
	LOG_DEBUG("seed: ", Seed);
}

void CGameRuleManager::RegisterPlayerHeartContainer(const int InstantID, const int InitialLimit)
{
	if (mPlayerHeartContainer.find(InstantID) != mPlayerHeartContainer.end())
		return;

	FPlayerHeartContainer container;
	container.Limit = InitialLimit;
	for (int i = 0; i < InitialLimit; ++i)
	{
		container.Hearts.push_back(FPlayerHeartData(EPlayerHeartType::Red, EPlayerHeartState::Full));
	}
	mPlayerHeartContainer.insert(std::make_pair(InstantID, container));
}

bool CGameRuleManager::FillHeart(const int ID, EPlayerHeartType Heart, EPlayerHeartState State)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || EPlayerHeartState::Empty == State)
		return false;

	int length = mPlayerHeartContainer[ID].Hearts.size() - 1;
	for (int i = length; i >= 0; --i)
	{
		if (EPlayerHeartState::Full != mPlayerHeartContainer[ID].Hearts[i].State)
		{
			mPlayerHeartContainer[ID].Hearts[i].State = 
				EPlayerHeartState::Empty == mPlayerHeartContainer[ID].Hearts[i].State ? EPlayerHeartState::Half : EPlayerHeartState::Full;

			if (EPlayerHeartState::Full == State)
				State = EPlayerHeartState::Half;
		}
	}

	if (EPlayerHeartState::Half == State)
	{
		length = mPlayerHeartContainer[ID].Hearts.size() - 1;
		for (int i = length; i >= 0; --i)
		{
			if (EPlayerHeartState::Full != mPlayerHeartContainer[ID].Hearts[i].State)
			{
				mPlayerHeartContainer[ID].Hearts[i].State =
					EPlayerHeartState::Empty == mPlayerHeartContainer[ID].Hearts[i].State ? EPlayerHeartState::Half : EPlayerHeartState::Full;
			}
		}
	}

	return true;
}

bool CGameRuleManager::DrainHeart(const int ID, EPlayerHeartType Heart, EPlayerHeartState State)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || EPlayerHeartState::Empty == State)
		return false;

	int length = mPlayerHeartContainer[ID].Limit;
	for (int i = 0; i < length; ++i)
	{
		if (EPlayerHeartState::Empty != mPlayerHeartContainer[ID].Hearts[i].State)
		{
			mPlayerHeartContainer[ID].Hearts[i].State =
				EPlayerHeartState::Full == mPlayerHeartContainer[ID].Hearts[i].State ? EPlayerHeartState::Half : EPlayerHeartState::Empty;

			if (EPlayerHeartState::Full == State)
				State = EPlayerHeartState::Half;
		}
	}
	if (EPlayerHeartState::Half == State)
	{
		length = mPlayerHeartContainer[ID].Limit;
		for (int i = 0; i < length; ++i)
		{
			if (EPlayerHeartState::Empty != mPlayerHeartContainer[ID].Hearts[i].State)
			{
				mPlayerHeartContainer[ID].Hearts[i].State =
					EPlayerHeartState::Full == mPlayerHeartContainer[ID].Hearts[i].State ? EPlayerHeartState::Half : EPlayerHeartState::Empty;
			}
		}
	}

	return true;
}

bool CGameRuleManager::AddContainerCapcity(const int ID, EPlayerHeartState State)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || mPlayerHeartContainer[ID].Hearts.size() >= HEART_MAX)
		return false;

	mPlayerHeartContainer[ID].Hearts.push_back(FPlayerHeartData(EPlayerHeartType::Red, State));
	mPlayerHeartContainer[ID].Limit++;

	return true;
}

bool CGameRuleManager::RemoveContainerCapcity(const int ID)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || mPlayerHeartContainer[ID].Hearts.size() < 1)
		return false;



	return true;
}
