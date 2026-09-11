#include "GameRuleManager.h"

#include "LogManager.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "../Data/GameDataManager.h"

#include "../Chapter.h"
#include "../Base/Character.h"
#include "../Base/Roombase.h"
#include "../Component/ItemContainer.h"

#include "../Data/RunGData.h"
#include "../Data/GameObjectStructure.h"

//아래 내용들 변수로 바꾸기
#define HEART_MAX 12
#define COIN_MAX 99
#define KEY_MAX 99
#define BOMB_MAX 99

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

void CGameRuleManager::AddCoin(int Add)
{
	mCoinCount += Add;
	if (mCoinCount > COIN_MAX)
		mCoinCount = COIN_MAX;

	LOG_DEBUG("동전을 ", std::to_string(Add), "만큼 얻었습니다.");
}

void CGameRuleManager::AddKey(int Add)
{
	mKeyCount += Add;
	if (mKeyCount > KEY_MAX)
		mKeyCount = KEY_MAX;
	LOG_DEBUG("열쇠를 ", std::to_string(Add), "만큼 얻었습니다.");
}

void CGameRuleManager::AddBomb(int Add)
{
	mBombCount += Add;
	if (mBombCount > BOMB_MAX)
		mBombCount = BOMB_MAX;
	LOG_DEBUG("폭탄을 ", std::to_string(Add), "만큼 얻었습니다.");
}

void CGameRuleManager::RemoveCoin(int Remove)
{
	mCoinCount -= Remove;
	if (mCoinCount < 0)
		mCoinCount = 0;
}

void CGameRuleManager::RemoveKey(int Remove)
{
	mKeyCount -= Remove;
	if (mKeyCount < 0)
		mKeyCount = 0;
}

void CGameRuleManager::RemoveBomb(int Remove)
{
	mBombCount -= Remove;
	if (mBombCount < 0)
		mBombCount = 0;
}

bool CGameRuleManager::CanUseCoin(int Count)
{
	return mCoinCount >= Count;
}

bool CGameRuleManager::CanUseKey(int Count)
{
	return mKeyCount >= Count;
}

bool CGameRuleManager::CanUseBomb(int Count)
{
	return mBombCount >= Count;
}

bool CGameRuleManager::SaveData(std::weak_ptr<class CChapter> targetChapter)
{
	if (targetChapter.expired())
		return false;

	FRunData d;
	FChapterData cd;
	cd.ChapterLevel = targetChapter.lock()->GetLevel();
	targetChapter.lock()->MakeChapterData(cd.Rooms);

	d.CurrentChapterLevel = cd.ChapterLevel;
	d.Chapters.push_back(cd);

	std::shared_ptr<CCharacter> chara = std::dynamic_pointer_cast<CCharacter>(targetChapter.lock()->GetPlayerCharacter().lock());
	FPlayerData pd = chara->MakeItemData();
	pd.ID = chara->GetGClassID();
	for (FPlayerHeartData container : mPlayerHeartContainer[chara->GetID()].RedContainer)
	{
		FHeartData hd;
		hd.Type = static_cast<int>(container.Type);
		hd.State = static_cast<int>(container.State);
		pd.Hearts.push_back(hd);
	}
	for (FPlayerHeartData container : mPlayerHeartContainer[chara->GetID()].OtherContainer)
	{
		FHeartData hd;
		hd.Type = static_cast<int>(container.Type);
		hd.State = static_cast<int>(container.State);
		pd.Hearts.push_back(hd);
	}
	//std::vector<FHeartData> Hearts;
	pd.Coins = mCoinCount;
	pd.Keys = mKeyCount;
	pd.Bombs = mBombCount;
	//방위치(좌표) , 방 내의 위치(좌표)
	pd.ChapterCoord = targetChapter.lock()->GetFocusedRoomCoord();
	pd.RoomCoord = chara->GetRoom().lock()->WorldPosToCoord(chara->GetWorldPos());
	
	d.Player = pd;

	CRunGData runGD;
	runGD.ContainData(d);

	runGD.WriteData(L"Run\\SaveFile");

	return true;
}

bool CGameRuleManager::LoadData()
{
	std::shared_ptr<CGameDataManager> mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
	if (!mgr->LoadDataFile<CRunGData>("SaveFile", EGDataType::Run, L"SaveFile"))
		return false;

	std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock());
	if (!chapter)
		return false;
	
	FRunData d = mgr->FindData<CRunGData>("SaveFile", EGDataType::Run).lock()->GetData();
	
	mCoinCount = d.Player.Coins;
	mKeyCount = d.Player.Keys;
	mBombCount = d.Player.Bombs;

	int id = chapter->GetPlayerCharacterID();

	mPlayerHeartContainer[id].RedContainer.clear();
	mPlayerHeartContainer[id].OtherContainer.clear();

	for (int i = 0; i < d.Player.Hearts.size(); ++i)
	{
		FHeartData heart = d.Player.Hearts[i];
		FPlayerHeartData hd((EPlayerHeartType)heart.Type, (EPlayerHeartState)heart.State);
		if(EPlayerHeartType::Red == hd.Type)
			mPlayerHeartContainer[id].RedContainer.push_back(hd);
		else
			mPlayerHeartContainer[id].OtherContainer.push_back(hd);
	}

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
