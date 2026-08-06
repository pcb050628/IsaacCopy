#include "Roombase.h"

#include "LogManager.h"

#include "../Manager/GameRuleManager.h"
#include "../Manager/GameClassContainer.h"

#include "../Chapter.h"
#include "World/MeshComponent.h"

#include "Unitbase.h"
#include "Obstaclebase.h"
#include "../Door.h"


CRoombase::CRoombase(ERoomType Type, ERoomShape Shape)
	:CGameObject(EObjectType::Room), mRoomType(Type), mShape(Shape)
{
}

CRoombase::CRoombase(const CRoombase& src)
	:CGameObject(src), mRoomType(src.mRoomType), mShape(src.mShape)
{}

CRoombase::CRoombase(CRoombase&& src) noexcept
	:CGameObject(std::move(src)), mRoomType(src.mRoomType), mShape(src.mShape)
{}

CRoombase::~CRoombase()
{
}

bool CRoombase::Init()
{
	mShadeMesh1 = CreateComponent<CMeshComponent>("Root");

	if (mShadeMesh1.expired())
		return false;

	mShadeMesh1.lock()->SetWorldScale(FVector2(1.f, 1.f));


	//객체 생성
	return true;
}

void CRoombase::Update(float DeltaTime)
{
	if (!mbIsRoomWin) //방 클리어 여부 검사
	{
		if (WinCheck())
		{
			WinRoom();
		}
	}
	CActor::Update(DeltaTime);
}

void CRoombase::SetEnableAll(bool Enable)
{
	for (std::weak_ptr<CDoor> door : mDoors)
	{
		if (door.expired())
			continue;

		door.lock()->SetEnable(Enable);
	}
	/*for (std::weak_ptr<CObstacle> obstacle : mObstacle)
	{
		if (obstacle.expired())
			continue;

		obstacle.lock()->SetEnable(Enable);
	}*/
	for (std::weak_ptr<CUnitbase> unit : mUnits)
	{
		if (unit.expired())
			continue;

		unit.lock()->SetEnable(Enable);
	}
}

void CRoombase::AdjustRoomPos()
{
	std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (mNearRooms.empty() || !chapter)
		return;

	FVector2 center = chapter->GetStartRoomCoord();
	int dist = 1000000;
	std::weak_ptr<CRoombase> targetRoom;
	std::list<std::pair<FVector2, std::weak_ptr<CRoombase>>>::iterator iter = mNearRooms.begin();
	std::list<std::pair<FVector2, std::weak_ptr<CRoombase>>>::iterator iterEnd = mNearRooms.end();
	for (; iter != iterEnd; ++iter)
	{
		int cal = abs(static_cast<int>(iter->first.x - center.x)) + abs(static_cast<int>(iter->first.y - center.y));
		if (cal < dist)
		{
			dist = cal;
			targetRoom = iter->second;
		}
	}
	FVector2 dir = mCoord - targetRoom.lock()->mCoord;
	//거리를 모르니까 일단 100으로 할까
	SetWorldPos(targetRoom.lock()->GetWorldPos() 
		+ FVector3(dir.x * mRoomCellMax.x * mRoomCellSize.x, dir.y * mRoomCellMax.x * mRoomCellSize.x, 0));
}

bool CRoombase::SetInitRoom(FVector2 Coord, const std::vector<std::pair<int, FVector2>>& Objs) //모양도 여기서 받아서 초기화 하기
{
	std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (!chapter)
		return false;

	mCoord = Coord;
	mInitInfo = Objs;
	//주변 방 찾아서 위치 조정하기
	//이동 기준이 될 방을 먼저 구해야함
	//기준이 되는 방은 중심 방향에 가까운 방
	//만약 두개 이상의 거리가 같은 경우 연결된 방 <- 생각해보니 연결된 두 방의 거리가 같으려면 두 방이 서로 마주보는 모양이여야 하는데 그러면 기준을 어디로 두든 문제 없음
	AdjustRoomPos();

	for (std::pair<int, FVector2> pair : Objs)
	{
		if (mRedFlag != FVector2(-1, -1))
		{
			if (pair.second >= mRedFlag)
			{
				//LOG_DEBUG("객체 생성 취소 | 좌표가 잘못되었습니다.: ", pair.second);
				continue;
			}
		}

		std::shared_ptr<CGameObject> gobj = CGameClassContainer::GetInst()->Instantiate(pair.first, pair.second, chapter->GetLevel()).lock();
		if (!gobj)
		{
			//LOG_DEBUG("객체 생성 실패 | 객체 ID: ", pair.first);
			continue;
		}

		//어느 방인지 알려주기
		switch (gobj->GetObjType())
		{
		case EObjectType::Monster: {
			std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(gobj);
			unit->SetRoom(GetThisPtr<CRoombase>());
			}			
			break;
		case EObjectType::Obstacle: {
			std::shared_ptr<CObstaclebase> obstacle = std::dynamic_pointer_cast<CObstaclebase>(gobj);
			obstacle->SetRoom(GetThisPtr<CRoombase>());
			}
			break;
		case EObjectType::Pickup:
			break;
		}

		//위치 조정하기
		FVector3 finalPos = GetWorldPos() + FVector3(pair.second.x - 13, pair.second.y - 7, 1) * ROOM_GRID_SIZE - ROOM_GRID_HALF;
		gobj->SetWorldPos(finalPos);
	}

	return true;
}

void CRoombase::Reset(bool HardReset)
{
	//InitInfo 를 이용해서 초기화를 하는데
	std::list<std::weak_ptr<CUnitbase>>::iterator iter = mUnits.begin();
	std::list<std::weak_ptr<CUnitbase>>::iterator iterEnd = mUnits.end();

	for (; iter != iterEnd;)
	{
		if (iter->expired())
		{
			++iter;
			continue;
		}
		std::shared_ptr<CUnitbase> unit = iter->lock();
		if (unit->GetIsTemporary())
		{
			std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
			chapter->ReturnGObj(unit);
			iter = mUnits.erase(iter);
			iterEnd = mUnits.end();
			continue;
		}

		if (!HardReset && unit->GetIsDead())
		{
			++iter;
			continue;
		}

		//살아있고 일시적인 객체가 아닌경우 초기화
		unit->Reset();

		++iter;
	}

	if (HardReset)
	{

	}
}

const FVector3 CRoombase::CoordToWorldPos(FVector2 Coord)
{
	//좌표기준으로 00이 좌하단
	//00에 그냥 위치를 곱하면 제자리니까 00에서 그리드크기 절반만큼 빼기
	//0, 0->-6, -4
	//셀 크기만큼 곱하기
	//-6, -4 -> -60, -40
	//벽이 있으니까 벽만큼의 오프셋을 줘야하는데
	//
	//방의 중심으로부터 이동하기
	FVector2 cal = ((Coord - mRoomCellMax / 2) * mRoomCellSize);
	cal.x += mRoomCellSize.x / 2;
	return GetWorldPos() + FVector3(cal.x, cal.y, 1);
}

const FVector2 CRoombase::GetUnitCoordInGrid(FVector3 WorldPos)
{
	FVector3 pos = WorldPos -= GetWorldPos();
	pos.x -= mRoomCellSize.x / 2;
	FVector2 result = FVector2(pos.x, pos.y) / mRoomCellSize;
	result += mRoomCellMax / 2;
	result.x = floor(result.x);
	result.y = floor(result.y);
	return result;
}

//나중에 수정해야하는 내용
//1. 플레이어 및 요청하는 유닛의 상태를 알고 상태에 따라 다르게 값을 계산해야함
//2. 날아다니는 유닛은 항상 장애물이 없음으로 판단
const bool CRoombase::CanGetToPlayerCharacter(FVector3 FromWorldPos)
{
	std::shared_ptr<CChapter> chpter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (!chpter)
		return false;

	std::shared_ptr<CUnitbase> pUnit = std::dynamic_pointer_cast<CUnitbase>(chpter->GetPlayerCharacter().lock());
	if (!pUnit)
		return false;

	FVector2 PlayerCoord = GetUnitCoordInGrid(pUnit->GetWorldPos());
	FVector2 FromCoord = GetUnitCoordInGrid(FromWorldPos);
	if (!CheckNearCell(FromCoord) || !CheckNearCell(PlayerCoord))
		return false;

	return true;
}

bool CRoombase::CheckNearCell(FVector2 Coord)
{
	for (int i = 0; i < 4; ++i)
	{
		FVector2 dest = Coord + CChapter::FourDirections[i];
		if (dest.x < 0 || dest.y < 0 || dest.x >= mRoomCellMax.x || dest.y >= mRoomCellMax.y)
			continue;

		int hash = CChapter::Coord2Hash(dest);
		if (EObjectType::Obstacle != mObjGridMap[hash])	
			return true;
	}
	return false;
}

void CRoombase::ConnectRoom(std::weak_ptr<CRoombase> Room)
{
	if (Room.expired())
	{
		//LOG_DEBUG("Room이 빈 객체입니다.");
		return;
	}

	mNearRooms.push_back(std::make_pair(Room.lock()->mCoord, Room));

}

void CRoombase::GenerateRoom(FVector2 Direction, int Min, int Max, int& Current) //이거 이대로 쓰면 안되겠다. 하위 객체들 생성하면 함수 오버라이드 해서 내용 수정하기
{
	if (Current > Max)
		return;

	//이 함수는 오버라이드 해서 다시 만들것임
	//방 모양에 따라서 진행하는 방향의 개수가 다르고 좌표 오프셋이 다르기 때문
	//

	std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	 
	FVector2 dest = mCoord + Direction;
	if (1 == chapter->GetIsValidCoord(dest)) //진행 방향에 이미 방이 있다면 그대로 이어간다.
	{
		std::weak_ptr<CRoombase> roomBase = chapter->GetRoom(dest);
		roomBase.lock()->GenerateRoom(Direction, Min, Max, Current);
		return;
	}

	//파일 읽어와서 방 만들어보자
	//1. 단 애셋으로 만들어
	//2. 파일을 쓰고 읽기가 잘 되는지 보기
	//3. 문제를 해결한 후 방 생성해보기
	//4. 방 생성까지 이상없이 되었다면 필요한 리소스들 불러와서 합쳐주기
	
	//CGameClassContainer 에서 생성을 시도
	//생성함수 내부에서 자동으로 파일을 읽음
	//어떤 방을 만들어야 하는지만 정해주면 알아서 해당 방에 사용가능한 파일을 골라서 읽음
	//원래는 인접한 방중 비밀방이 있는 경우에는 방의 비밀방을 가지 못하게 막는 구조의 방은 사용하면 안되지만
	//일단은 주변방에는 관련없이 모든 방을 사용 가능하게 만들고
	//만약 오브젝트가 길을 막는 경우에는 해당 오브젝트를 파괴하자
	std::weak_ptr<CRoombase> generatedRoom = std::dynamic_pointer_cast<CRoombase>(CGameClassContainer::GetInst()->Instantiate(10, dest).lock()); //생성 후 내부에서 방 연결하기
	std::shared_ptr<CRoombase> room = generatedRoom.lock();
	if (!room) 
		return;

	//방 위치 오프셋 줘야함
	//어디서 주지
	//위치 오프셋이 정해지는 규칙을 보면
	//1. 주변방이 있는 경우만 위치 오프셋이 필요함
	//2. 주변방이 한개 일때는 해당 방으로부터 방 크기의 절반만큼만 이동하면됨
	//3. 주변방이 2개 이상일때는 인접한 방들의 크기들과 현재방 크기를 이용해서 오프셋을 줘야함
	//3-1. 현재 방의 크기가 인접한 방들과 같은 경우
	//		ㄴ방이 한개일때와 같음
	//3-2. 현재의 방의 크기가 인접한 방들과 다를 경우
	//		ㄴ시작 방을 찾고 해당 방향에 있는 방을 기준으로 이동 한 후 다른 방향에 있는 방들을 순차적으로 다 이동시킨다.
	//		ㄴ시작방 + 오프셋=>현재 위치 재설정 -> 현재 위치 + 오프셋=>인접한 방 위치 재설정 -> 인접한 방에서 다시 반복
	//
	//생긴걸 보니까 호출자체는 CreateRoom 에서 하고 실제로 위치 재조정하는 내용은 Roombase 내부에 둬야겠다.

	++Current;
	for (int i = 0; i < 4; i++)//어떤 방인지에 따라 다르지만 기본으로는 4방향
	{
		if (CChapter::FourDirections[i] == -Direction)
			continue;
		float random = CGameRuleManager::GetInst()->GenerateRandom();
		if (random < 0.5f)
			continue;

		FVector2 next = dest + CChapter::FourDirections[i];
		if (chapter->GetIsValidCoord(next) != 0)
		{
			room->GenerateRoom(CChapter::FourDirections[i], Min, Max, Current);
		}
	}
	
	//LShape 인 경우
	//TR | RT 은 같은 방향을 가리킨다.
	return;
}