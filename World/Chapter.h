#pragma once
#include "World\World.h"
#include "Base/Roombase.h"
#include "Base/Unitbase.h"
#include "Base/Obstaclebase.h"
#include "Base/Tear.h"

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
	int mFocusedRoomHash = -1;
	int mPrevRoomHash = -1;
	std::unordered_map<int, std::weak_ptr<CRoombase>> mRoomMap;
	//리스트 써서 맵->리스트 로 만들면 좋을거같은ㄷ
	//접근은 클래스 아이디로 하고 해당 클래스 아이디 리스트 내부에서 하나 꺼내오면 되니까
	std::weak_ptr<CUnitbase> mPlayerCharacter;
	std::unordered_map<int, std::list<std::weak_ptr<CUnitbase>>> mUnitsActivate;
	std::unordered_map<int, std::list<std::weak_ptr<CUnitbase>>> mUnitsDeactivate;
	std::unordered_map<int, std::list<std::weak_ptr<CObstaclebase>>> mObstaclesActivate;
	std::unordered_map<int, std::list<std::weak_ptr<CObstaclebase>>> mObstaclesDeactivate;
	std::unordered_map<int, std::weak_ptr<CTear>> mTearsActivate;
	std::unordered_map<int, std::weak_ptr<CTear>> mTearsDeactivate;

	//눈물 발사기(컴포넌트) <- 이거 필요한가? 어짜피 보관되는 위치도 Chapter 고 다 여기서 받아오는데 기능도 전부 Chapter 에 의탁해버리면?
	//							챕터가 너무 방대해지긴하네 이미 다른 유닛들의 관리도 책임지고 있는데 책임을 합치는것이면 모르겠지만 다른 객체들과 다르게
	//							눈물의 경우는 시도때도 없이 생성되고 반납되기를 반복하니
	//1. 입력받을 내용(발사한 객체, 크기, 방향, 높이, 거리, 속도, 충돌시 효과(함수 포인터))
	//2. 필요한 기능(객체 생성 및 가져오기, 객체 반납받기, 발사 위치 조정, 

	//방에 종속되지않는 객체
	//카메라 및 UI 역할을 겸함 / 각각의 역할을 하는 액터 컴포넌트를 각각 지님 각각각각각각각각
	//포커스된 방을 비추고
	//UI On/Off 시 움직이는 효과는 이 객체에 있음
	//아직 만들지 않았고 만들어야함
	std::weak_ptr<class CChapterSystemActor> mChapterManagementActor;
	std::vector<std::weak_ptr<CActor>> mWalls; //벽 12개 만들기
	std::vector<std::weak_ptr<class CDoor>> mDoors; // 문 8개

	/// <summary>
	/// 문을 게임오브젝트로 해야할까 ? 
	/// 게임ㅇ ㅗ브젝트의 기준은 무엇인가
	/// 지금까지의 기준으로는 인스턴싱이 필요한지가 기준인데
	/// 문의 경우는 그렇지않기는 하다 
	/// 인스턴시에이트가 필요없는데 게임 오브젝트를 상속받은 객체가 있나?
	/// 눈물이 있네
	/// 지금 보니까 눈물의 경우는 조금 다른데 
	/// 눈물은 여러개 생성되는 객체라서 인스턴시에이트의 문제라기보다는 해당 객체 아이디를 통한 관리를 위해 게임 오브젝트를 상속받았다
	/// 따라서 문의 경우, 두가지 모두 해당하지 않기 때문에 게임 오브젝트를 상속받지않게 만들도록 하겠다.
	/// </summary>

	int mChapterLevel = 1;
	int mRoomRowMax = 0;
	int mRoomColMax = 0;

	FTimerHandle mRoomEnterTimer;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	void GenerateWallAndDoor();
	void GenerateRoom();
	void InitialSetting();
	void SettingFocus();

	void SetPlayerPos();

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
		if (!generatedRoom)
			return std::weak_ptr<CRoombase>();
		std::vector<std::pair<int, FVector2>> GObjs; //<- 이건 파일 만들기 전 일단 테스트용
		//generatedRoom->GetRoomType()
		//현재 생성될 방의 타입을 보내서 해당 방에 맞는 파일 읽어오기
		//SetInitRoom 에 방 모양도 입력해주기
		if (-1 == mFocusedRoomHash)
		{
			mFocusedRoomHash = Coord2Hash(Coord);
			mRoomMap[mFocusedRoomHash] = generatedRoom;
		}
		else
		{
			mRoomMap[Coord2Hash(Coord)] = generatedRoom;
		}

		generatedRoom->SetCoord(Coord);
		for (int i = 0; i < 4; i++)
		{
			int dest = Coord2Hash(Coord + FourDirections[i]);
			if (!mRoomMap[dest].expired())
			{
				generatedRoom->ConnectRoom(mRoomMap[dest]);
				mRoomMap[dest].lock()->ConnectRoom(generatedRoom);
			}
		}
		if (!generatedRoom->SetInitRoom(GObjs))
		{
			generatedRoom->Destroy();
			return std::weak_ptr<CRoombase>();
		}
		return generatedRoom;
	}
	template<typename T>
	std::weak_ptr<T> CreateCharacter(const std::string& Name, FVector2 Coord)
	{
		//플레이어가 이미 있는경우 널 반환
		if (!mPlayerCharacter.expired())
			return std::weak_ptr<T>();

		std::weak_ptr<T> unit = CreateActor<T>(Name);
		if (unit.expired())
			return std::weak_ptr<T>();
		mPlayerCharacter = std::dynamic_pointer_cast<CUnitbase>(unit.lock());
		std::shared_ptr<CUnitbase> ub = std::dynamic_pointer_cast<CUnitbase>(unit.lock());
		ub->SetWorldPos(mRoomMap[mFocusedRoomHash].lock()->CoordToWorldPos(Coord));
		ub->SetRoom(mRoomMap[mFocusedRoomHash].lock());
		return unit;
	}
	template<typename T>
	std::weak_ptr<T> CreateMonster(const std::string& Name, const int GObjID, FVector2 Coord)
	{
		if (!mUnitsDeactivate[GObjID].empty())
		{
			std::shared_ptr<CUnitbase> unit = mUnitsDeactivate[GObjID].front().lock();
			std::shared_ptr<T> mon = std::dynamic_pointer_cast<T>(unit);
			mUnitsDeactivate[GObjID].pop_front();
			mUnitsActivate[GObjID].push_back(unit);

			unit->Reset(true);
			unit->SetWorldPos(mRoomMap[mFocusedRoomHash].lock()->CoordToWorldPos(Coord));
			unit->SetRoom(mRoomMap[mFocusedRoomHash].lock());
			mRoomMap[mFocusedRoomHash].lock()->RegisterGObj(unit, Coord);
			return mon;
		}

		std::weak_ptr<T> unit = CreateActor<T>(Name);
		if (unit.expired())
			return std::weak_ptr<T>();

		std::shared_ptr<CUnitbase> ub = std::dynamic_pointer_cast<CUnitbase>(unit.lock());
		mUnitsActivate[GObjID].push_back(ub);

		ub->SetWorldPos(mRoomMap[mFocusedRoomHash].lock()->CoordToWorldPos(Coord));
		ub->SetRoom(mRoomMap[mFocusedRoomHash].lock());
		mRoomMap[mFocusedRoomHash].lock()->RegisterGObj(ub, Coord);
		return unit;
	}
	template<typename T>
	std::weak_ptr<T> CreateObstacle(const std::string& Name, const int GObjID, FVector2 Coord)
	{
		if (!mObstaclesDeactivate[GObjID].empty())
		{
			std::shared_ptr<CObstaclebase> obstacle = mObstaclesDeactivate[GObjID].front().lock();
			std::shared_ptr<T> ob = std::dynamic_pointer_cast<T>(obstacle);
			mObstaclesDeactivate[GObjID].pop_front();
			mObstaclesActivate[GObjID].push_back(obstacle);

			obstacle->Reset(true);
			obstacle->SetWorldPos(mRoomMap[mFocusedRoomHash].lock()->CoordToWorldPos(Coord));
			obstacle->SetRoom(mRoomMap[mFocusedRoomHash].lock());
			mRoomMap[mFocusedRoomHash].lock()->RegisterGObj(obstacle, Coord);
			return ob;
		}

		std::weak_ptr<T> obstacle = CreateActor<T>(Name);
		if (obstacle.expired())
			return std::weak_ptr<T>();
		
		std::shared_ptr<CObstaclebase> ob = std::dynamic_pointer_cast<CObstaclebase>(obstacle.lock());
		mObstaclesActivate[GObjID].push_back(ob);

		ob->SetWorldPos(mRoomMap[mFocusedRoomHash].lock()->CoordToWorldPos(Coord));
		ob->SetRoom(mRoomMap[mFocusedRoomHash].lock());
		mRoomMap[mFocusedRoomHash].lock()->RegisterGObj(ob, Coord);
		return obstacle;
	}
	template<typename T>
	std::weak_ptr<CGameObject> MakeObject(const std::string& Name, enum class EObjectType Type, const int GObjID, FVector2 Coord)//좌표는 무조건 받아야 하고, 
	{
		//함수에 챕터내의 방 좌표, 방 내부좌표 받기
		//현재 생성되고 있는 객체들은 전부 포커싱된 방에 생성되게 만들어져 있으므로 여러방을 동시에 생성하는데에 부적합함
		switch (Type)//각 타입별 생성 함수 만들어 주기
		{
		case EObjectType::Room:
			return CreateRoom<T>(Name, Coord);
		case EObjectType::PlayerCharacter:
			return CreateCharacter<T>(Name, Coord);
		case EObjectType::Monster:
			return CreateMonster<T>(Name, GObjID, Coord);
		case EObjectType::Obstacle:
			return CreateObstacle<T>(Name, GObjID, Coord);
		}

		return std::weak_ptr<CGameObject>();
	}

	std::weak_ptr<CTear> GetTear()
	{
		if (mTearsDeactivate.empty())
		{
			std::shared_ptr<CTear> tear = CreateActor<CTear>("Tear").lock();
			if (!tear)
				return std::weak_ptr<CTear>();
			mTearsActivate[tear->GetID()] = tear;
			return tear;
		}
		else
		{
			for (std::pair<int, std::weak_ptr<CTear>> tear : mTearsDeactivate)
			{
				if (!tear.second.expired())
				{
					mTearsDeactivate.erase(tear.first);
					mTearsActivate[tear.first] = tear.second;
					tear.second.lock()->Reset(true);
					return tear.second;
				}
			}
		}

		return std::weak_ptr<CTear>();
	}

	//받은 오브젝트 반납하기 - 오브젝트 비활성화하고 방에서 분리한 해제안하고 들고있음
	//눈물은 ClassID 가 아니라 오브젝트 아이디
	bool ReturnGObj(std::weak_ptr<CGameObject> Obj);

public:
	void MoveRoom(FVector2 Dir);


public:
	//좌표가 잘못됐으면 0 좌표에 방이 있으면 1 아무것도 없으면 2
	const int GetIsValidCoord(const FVector2& Coord);
	std::weak_ptr<CRoombase> GetRoom(FVector2 Coord);
	std::weak_ptr<CRoombase> GetFocusedRoom();
	const int GetRowMax() const { return mRoomRowMax; }
	const int GetColMax() const { return mRoomColMax; }

	const int GetLevel() const { return mChapterLevel; }

	const FVector2 GetStartRoomCoord() { return FVector2(static_cast<float>(mRoomRowMax / 2), static_cast<float>(mRoomColMax / 2)); }

	std::weak_ptr<CActor> GetPlayerCharacter() { return mPlayerCharacter; }
	const FVector2 GetFocusedRoomCoord() const { return Hash2Coord(mFocusedRoomHash); }

public:
	static const FVector2 FourDirections[4];
	static const FVector2 RoomWorldSize;
	static const FVector2 EightDirections[8];
};

