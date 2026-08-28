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
	for (int i = 0; i < InitialLimit; ++i)
	{
		container.RedContainer.push_back(FPlayerHeartData(EPlayerHeartType::Red, EPlayerHeartState::Full));
	}
	mPlayerHeartContainer.insert(std::make_pair(InstantID, container));
	CallOnHeartUpdate();
}

bool CGameRuleManager::FillHeart(const int ID, EPlayerHeartType Heart, EPlayerHeartState State)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || EPlayerHeartState::Empty == State)
		return false;

	if (EPlayerHeartType::Red == Heart)
	{
		if (EPlayerHeartState::Full == mPlayerHeartContainer[ID].RedContainer.back().State)
			return false;

		std::list<FPlayerHeartData>::iterator iter = mPlayerHeartContainer[ID].RedContainer.begin();
		std::list<FPlayerHeartData>::iterator iterEnd = mPlayerHeartContainer[ID].RedContainer.end();

		for (; iter != iterEnd; ++iter)
		{
			if (EPlayerHeartState::Full != iter->State)
			{
				EPlayerHeartState result = State - (-iter->State);
				if (EPlayerHeartState::Empty == iter->State)
				{
					iter->State = State;
				}
				else 
				{
					iter->State = EPlayerHeartState::Full;
				}
				State = result;
				break;
			}
		}

		if (EPlayerHeartState::Half == State)
		{
			iter = mPlayerHeartContainer[ID].RedContainer.begin();
			iterEnd = mPlayerHeartContainer[ID].RedContainer.end();
			for (; iter != iterEnd; ++iter)
			{
				if (EPlayerHeartState::Empty == iter->State)
				{
					iter->State = State;
					break;
				}
				else if (EPlayerHeartState::Half == iter->State)
				{
					iter->State = EPlayerHeartState::Full;
					break;
				}
			}
		}
	}
	CallOnHeartUpdate();
	return true;
}

bool CGameRuleManager::DrainHeart(const int ID, EPlayerHeartState State)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || EPlayerHeartState::Empty == State)
		return false;

	std::list<FPlayerHeartData>::reverse_iterator iter = mPlayerHeartContainer[ID].RedContainer.rbegin();
	std::list<FPlayerHeartData>::reverse_iterator iterEnd = mPlayerHeartContainer[ID].RedContainer.rend();
	for (; iter != iterEnd; ++iter)
	{
		if (EPlayerHeartState::Empty != iter->State)
		{
			EPlayerHeartState resultState = State - iter->State;
			iter->State = iter->State - State;
			State = resultState;
			break;
		}
	}
	if (EPlayerHeartState::Half == State)
	{
		iter = mPlayerHeartContainer[ID].RedContainer.rbegin();
		iterEnd = mPlayerHeartContainer[ID].RedContainer.rend();
		for (; iter != iterEnd; ++iter)
		{
			if (EPlayerHeartState::Empty != iter->State)
			{
				iter->State = iter->State - State;
			}
		}
	}
	CallOnHeartUpdate();
	return true;
}

bool CGameRuleManager::AddRedContainerCapcity(const int ID, EPlayerHeartState State)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || mPlayerHeartContainer[ID].RedContainer.size() >= HEART_MAX)
		return false;

	mPlayerHeartContainer[ID].RedContainer.push_back(FPlayerHeartData(EPlayerHeartType::Red, State));
	CallOnHeartUpdate();
	return true;
}

bool CGameRuleManager::RemoveRedContainerCapcity(const int ID)
{
	if (mPlayerHeartContainer.find(ID) == mPlayerHeartContainer.end() || mPlayerHeartContainer[ID].RedContainer.size() < 1)
		return false;

	mPlayerHeartContainer[ID].RedContainer.pop_back();
	CallOnHeartUpdate();
	return true;
}

void CGameRuleManager::CallOnHeartUpdate()
{
	for (std::pair<std::string, std::function<void(int, FPlayerHeartContainer)>> pair : OnHeartUpdate)
	{
		for (std::pair<int, FPlayerHeartContainer> container : mPlayerHeartContainer)
		{
			pair.second(container.first, container.second);
		}
	}
}
