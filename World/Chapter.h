#pragma once
#include "World\World.h"
#include "Base/Roombase.h"
#include "Base/Unitbase.h"
#include "Base/Obstaclebase.h"

enum class ERoomType;
enum class ERoomShape;

#define DIR_RIGHT 0
#define DIR_LEFT  1
#define DIR_DOWN  2
#define DIR_TOP   3

class CRoombase;
class CUnitbase;
class CObstaclebase;

class CChapter :
	public CWorld
{
public:
	CChapter();
	virtual ~CChapter();

protected:
	//RoomMap 은 맵으로 쓰는 이유가 인접한 다른 방에 접근할 때 너무 불편함
	std::weak_ptr<CRoombase> mFocusedRoom;
	std::unordered_map<int, std::weak_ptr<CRoombase>> mRoomMap;
	//구조 변경하기 / 맵 -> 리스트 / 불을 키로 쓰는것보다 리스트에 불 유닛 쌍으로 넣고 불값으로 정렬하는게 나을듯
	std::unordered_map<int, std::pair<bool, std::weak_ptr<CUnitbase>>> mUnits;
	std::unordered_map<int, std::pair<bool, std::weak_ptr<CObstaclebase>>> mObstacles;

	//방에 종속되지않는 객체
	//카메라 및 UI 역할을 겸함 / 각각의 역할을 하는 액터 컴포넌트를 각각 지님 각각각각각각각각
	//포커스된 방을 비추고
	//UI On/Off 시 움직이는 효과는 이 객체에 있음
	//아직 만들지 않았고 만들어야함
	std::weak_ptr<class CGameSystemActor> mGameSystemActor; 

	int mChapterNumber = 1;
	int mRoomRowMax = 0;
	int mRoomColMax = 0;


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	void GenerateRoom();

public:
	static int Coord2Hash(FVector2 Coord)
	{
		int x = static_cast<int>(Coord.x);
		int y = static_cast<int>(Coord.y);

		x = x > 9 ? x * 10 : x * 100;
		return 10000 + x + y;
	}
	static FVector2 Hash2Coord(int Key)
	{
		Key -= 10000;
		if (Key < 1000)
		{
			int x = Key / 100;
			int y = Key - x * 100;
			return FVector2(static_cast<float>(x), static_cast<float>(y));
		}
		else
		{
			int x = ((Key / 1000) * 10) + Key / 100;
			int y = Key - x * 10;
			return FVector2(static_cast<float>(x), static_cast<float>(y));
		}
	}
	//bool GameInitialize();
	//아래에 있는 템플릿 함수들은 각각 리소스 파일을 받을 수 있어야함
	//리소스 파일의 내용은
	//1. 이름은 필요없고 스탯같은것도 필요없고 분명 뭔가 필요는 할텐데
	//2. 방은 객체와 좌표를 각각 저장해야하고 객체의 아이디를 알면됨
	//3. 객체는 상태를 저장해야하고 상태의 종류는 객체마다 다름
	//4. 초기화를 생각해보자
	//		방을 생성을 하는데 방의 모양과 내부 객체들의 위치를 알아야함
	//		객체들을 생성할때는 딱히 알아야 하는게 없네

	//방은 항상 새로 생성
	template<typename T>
	std::weak_ptr<CRoombase> CreateRoom(std::string Name, FVector2 Coord)
	{
		if (Coord.x >= mRoomRowMax || Coord.x < 0 || Coord.y >= mRoomColMax || Coord.y < 0 || !GetRoom(Coord).expired())
		{
			return std::weak_ptr<CRoombase>();
		}

		std::weak_ptr<T> room = CreateActor<T>(Name);
		if (room.expired())
			return std::weak_ptr<CRoombase>();

		std::shared_ptr<CRoombase> generatedRoom = std::dynamic_pointer_cast<CRoombase>(room.lock());
		std::vector<std::pair<int, FVector2>> GObjs;
		//generatedRoom->GetRoomType()
		//현재 생성될 방의 타입을 보내서 해당 방에 맞는 파일 읽어오기
		//SetInitRoom 에 방 모양도 입력해주기
		if (!generatedRoom->SetInitRoom(Coord, GObjs))
			return std::weak_ptr<CRoombase>();

		mRoomMap[Coord2Hash(Coord)] = generatedRoom;
		for (int i = 0; i < 4; i++)
		{
			int dest = Coord2Hash(Coord + FourDirections[i]);
			if (!mRoomMap[dest].expired())
			{
				generatedRoom->ConnectRoom(mRoomMap[dest]);
				mRoomMap[dest].lock()->ConnectRoom(generatedRoom);
			}
		}
		return std::dynamic_pointer_cast<CRoombase>(room.lock());
	}
	template<typename T>
	std::weak_ptr<T> CreateUnit(const std::string& Name)
	{
		std::unordered_map<int, std::pair<bool, std::weak_ptr<CUnitbase>>>::iterator iter = mUnits.begin();
		std::unordered_map<int, std::pair<bool, std::weak_ptr<CUnitbase>>>::iterator iterEnd = mUnits.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second.first || !std::dynamic_pointer_cast<T>(iter->second.second.lock()))
				continue;

			std::shared_ptr<CUnitbase> unit = iter->second.second.lock();
			unit->Reset(true);
			iter->second.first = true;
			return std::dynamic_pointer_cast<T>(unit);
		}

		std::weak_ptr<T> unit = CreateActor<T>(Name);
		if (!unit.expired())
		{
			std::shared_ptr<CUnitbase> ub = std::dynamic_pointer_cast<CUnitbase>(unit.lock());
			mUnits[ub->GetID()] = std::make_pair(true, ub);
		}
		return unit;
	}
	template<typename T>
	std::weak_ptr<T> CreateObstacle(const std::string& Name)
	{
		std::unordered_map<int, std::pair<bool, std::weak_ptr<CObstaclebase>>>::iterator iter = mObstacles.begin();
		std::unordered_map<int, std::pair<bool, std::weak_ptr<CObstaclebase>>>::iterator iterEnd = mObstacles.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second.first || !std::dynamic_pointer_cast<T>(iter->second.second.lock()))
				continue;

			std::shared_ptr<CObstaclebase> obstacle = iter->second.second.lock();
			obstacle->Reset(true);
			iter->second.first = true;
			return std::dynamic_pointer_cast<T>(obstacle);
		}

		std::weak_ptr<T> obstacle = CreateActor<T>(Name);
		if (!obstacle.expired())
		{
			std::shared_ptr<CObstaclebase> ob = std::dynamic_pointer_cast<CObstaclebase>(obstacle.lock());
			mObstacles[ob->GetID()] = std::make_pair(true, ob);
		}
		return obstacle;
	}
	template<typename T>
	std::weak_ptr<class CGameObject> MakeObject(const std::string& Name, enum class EObjectType Type, FVector2 Coord)//좌표는 무조건 받아야 하고, 
	{
		switch (Type)//각 타입별 생성 함수 만들어 주기
		{
		case EObjectType::Room:
			return CreateRoom<T>(Name, Coord);
		case EObjectType::PlayerCharacter:
			break;
		case EObjectType::Monster:
			return CreateUnit<T>(Name);
		case EObjectType::Obstacle:
			return CreateObstacle<T>(Name);
		case EObjectType::Door:
			break;
		}

		return std::weak_ptr<CGameObject>();
	}

	bool ReturnGObj(std::weak_ptr<CGameObject> Obj);

	//좌표가 잘못됐으면 0 좌표에 방이 있으면 1 아무것도 없으면 2
	const int GetIsValidCoord(const FVector2& Coord);
	std::weak_ptr<CRoombase> GetRoom(FVector2 Coord);
	const int GetRowMax() const { return mRoomRowMax; }
	const int GetColMax() const { return mRoomColMax; }

public:
	static FVector2 FourDirections[4];
	static FVector2 EightDirections[8];
};

