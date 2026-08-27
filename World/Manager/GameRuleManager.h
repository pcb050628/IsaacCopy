#pragma once
#include "EngineInfo.h"
#include "../UnitData.h"

class CGameObject;
class CGameRuleManager
{
	Singleton(CGameRuleManager);
private: //일단 다 모아두고 모아 둘 필요가 없으면 흩어놓기
	//0. 전체
	std::random_device mRd;
	std::mt19937 mGen;
	std::uniform_real_distribution<float> mfDistribution;

	std::map<int, int> ObstacleDestroyDropPercentage; //장애물 파괴시 / 객체마다 다름 / 그냥 객체에 넣어놓ㅇ르까? 여기 놨을때 더 좋은 이유는 통제가 쉽다는건데 안좋은건 호출이 많을 수 있다는거고
	std::map<int, int> PickUpDropPercetages;
	std::vector<std::map<int, int>> ItemPoolPercentages;
	int PlayerItemWeightStack = 0; //아이템 가중치 스택 / 너무 좋은템이나 구린템이 연속으로 나오지않게함
	//1. 방에 대해서
	std::map<int, std::function<void()>> OnRoomEnter; //만들어 놓고 보니까 안쓸거같긴한데 혹시 모르니까 가만두기 / 나중에 거의 다 완성하고도 안쓸것같으면 치우기
	std::map<int, std::function<void()>> OnRoomExit;
	std::map<int, std::function<void()>> OnRoomClear;

	//2. 오브젝트에 대해서
	
	//3. 플레이어에 대해서
	EPlayerHeartType mPlayerHeartDrainPriority = EPlayerHeartType::End;
	std::map<int, FPlayerHeartContainer> mPlayerHeartContainer;

public:
	bool Init();

	void SetRandomSeed(unsigned int Seed = 1);
	float GenerateRandomF() { return mfDistribution(mGen); }
	int GenerateRandomI() { return static_cast<int>(mfDistribution(mGen) * 100); }

	const int GetItemWeightStack() const { return PlayerItemWeightStack; }

	void RegisterPlayerHeartContainer(const int InstantID, const int InitialLimit = 3);
	bool FillHeart(const int ID, EPlayerHeartType Heart, EPlayerHeartState State = EPlayerHeartState::Half); //체력 채우기
	bool DrainHeart(const int ID, EPlayerHeartType Heart, EPlayerHeartState State = EPlayerHeartState::Half); //체력 빼기
	bool AddContainerCapcity(const int ID, EPlayerHeartState State = EPlayerHeartState::Full); //칸 늘리기
	bool RemoveContainerCapcity(const int ID); //칸 줄이기

	template<typename T>
	void RegisterRoomEnterFunc(std::shared_ptr<T> Obj, void(T::*Func)())
	{
		std::shared_ptr<CGameObject> gobj = std::dynamic_pointer_cast<CGameObject>(Obj);
		if (!gobj)
			return;

		OnRoomEnter[gobj->GetID()] = std::bind(Func, Obj.get());
	}
	template<typename T>
	void RegisterRoomExitFunc(std::shared_ptr<T> Obj, void(T::* Func)())
	{
		std::shared_ptr<CGameObject> gobj = std::dynamic_pointer_cast<CGameObject>(Obj);
		if (!gobj)
			return;

		OnRoomExit[gobj->GetID()] = std::bind(Func, Obj.get());
	}
	template<typename T>
	void RegisterRoomClearFunc(std::shared_ptr<T> Obj, void(T::* Func)())
	{
		std::shared_ptr<CGameObject> gobj = std::dynamic_pointer_cast<CGameObject>(Obj);
		if (!gobj)
			return;

		OnRoomClear[gobj->GetID()] = std::bind(Func, Obj.get());
	}

	void DisregisterRoomEnterFunc(const int ID)
	{
		if (!OnRoomEnter.contains(ID))
			return;

		OnRoomEnter.erase(ID);
	}
	void DisregisterRoomExitFunc(const int ID)
	{
		if (!OnRoomExit.contains(ID))
			return;

		OnRoomExit.erase(ID);
	}
	void DisregisterRoomClearFunc(const int ID)
	{
		if (!OnRoomClear.contains(ID))
			return;

		OnRoomClear.erase(ID);
	}

};

