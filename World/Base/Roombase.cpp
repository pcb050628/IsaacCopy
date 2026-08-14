#include "Roombase.h"

#include "Device.h"
#include "LogManager.h"

#include "../Manager/GameRuleManager.h"
#include "../Manager/GameClassContainer.h"

#include "../Chapter.h"
#include "World/MeshComponent.h"

#include "Unitbase.h"
#include "Obstaclebase.h"
#include "Pickup.h"
#include "../Door.h"


CRoombase::CRoombase(ERoomType Type, ERoomShape Shape)
	:CGameObject(EObjectType::Room), mRoomType(Type), mShape(Shape)
{
}

CRoombase::CRoombase(const CRoombase& src)
	:CGameObject(src), mRoomType(src.mRoomType), mShape(src.mShape)
{
}

CRoombase::CRoombase(CRoombase&& src) noexcept
	:CGameObject(std::move(src)), mRoomType(src.mRoomType), mShape(src.mShape)
{
}

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

void CRoombase::CalculateSize()
{
	//1300x700 : 방의 넓이 1100 x 530  : 렌더 넓이 1400 x 800
	//이미지로 50픽셀 만큼 차이인데 234.f, 155.f  468x310 짜리 이미지를 1400:800으로 폈으니까
	//3.f x 2.5f 배율 150x125 => 방의 넓이는 1100x550 셀의 크기는 84.61538461538462 x 78.57142857142857

	FResolution resol = CDevice::GetInst()->GetResolution();
	FVector2 magnification;
	magnification.x = resol.Width / mRoomImageSize.x;
	magnification.y = resol.Height / mRoomImageSize.y;

	FVector2 wallSize = magnification * 50.f;
	mRoomSize = FVector2(resol.Width, resol.Height) - wallSize * 2;
	mRoomCellSize = mRoomSize / mRoomCellMax;
}

void CRoombase::AdjustRoomPos()
{
	std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (mNearRooms.empty() || !chapter)
		return;

	FVector2 center = chapter->GetStartRoomCoord();
	int dist = INT_MAX;
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
		+ FVector3(dir.x * mRoomCellMax.x * mRoomCellSize.x * 1.5f, dir.y * mRoomCellMax.y * mRoomCellSize.y * 1.5f, 0));
}

void CRoombase::RoomSetting()
{
	Reset(!mbIsRoomWin);
}

void CRoombase::RoomDisenable() //방 비활성화 시 아직 등록되어있는 오브젝트들의 아이디와 좌표를 저장
{
	for (std::weak_ptr<CGameObject> obj : mMonsterList)
	{
		if (obj.expired())
			continue;

		if (!obj.lock()->GetIsTemporary())
		{
			mMonsterData[CChapter::Coord2Hash(WorldPosToCoord(obj.lock()->GetWorldPos()))] = obj.lock()->GetGObjID();
		}

		obj.lock()->ReturnToChapter();
	}
	mMonsterList.clear();

	for (std::weak_ptr<CGameObject> obj : mObstacleList)
	{
		if (obj.expired())
			continue;

		if (!obj.lock()->GetIsTemporary())
		{
			mObstacleData[CChapter::Coord2Hash(WorldPosToCoord(obj.lock()->GetWorldPos()))] = obj.lock()->GetGObjID();
		}

		obj.lock()->ReturnToChapter();
	}
	mObstacleList.clear();

	for (std::weak_ptr<CGameObject> obj : mPickupList)
	{
		if (obj.expired())
			continue;

		if (!obj.lock()->GetIsTemporary())
		{
			mPickupData[CChapter::Coord2Hash(WorldPosToCoord(obj.lock()->GetWorldPos()))] = obj.lock()->GetGObjID();
		}

		obj.lock()->ReturnToChapter();
	}
	mPickupList.clear();

	SetEnable(false);
	SetRenderEnable(false);
}

bool CRoombase::SetInitRoom(const std::vector<std::pair<int, FVector2>>& InitData) //모양도 여기서 받아서 초기화 하기
{
	std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (!chapter)
		return false;

	mInitData = InitData;

	//여기서 클리어 시 보상 설정하기
	mbIsRoomWin = false;

	AdjustRoomPos();

	return true;
}

void CRoombase::Reset(bool HardReset)
{
	std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (!chapter)
		return;

	if (HardReset) //InitData 로 생성
	{
		for (std::pair<int, FVector2> pair : mInitData)
		{
			if (mRedFlag != FVector2(-1, -1))
			{
				if (pair.second >= mRedFlag)
				{
					//LOG_DEBUG("객체 생성 취소 | 좌표가 잘못되었습니다.: ", pair.second);
					continue;
				}
			}

			std::shared_ptr<CGameObject> gobj = CGameClassContainer::GetInst()->Instantiate(pair.first, pair.second, false, chapter->GetLevel()).lock();
			if (!gobj)
			{
				//LOG_DEBUG("객체 생성 실패 | 객체 ID: ", pair.first);
				continue;
			}

			auto t = gobj->GetObjType();
			switch (t)
			{
			case EObjectType::Monster:{
				std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(gobj);
				unit->SetRoom(GetThisPtr<CRoombase>());
				RegisterGObj(gobj, pair.second);
				unit->SetWorldPos(CoordToWorldPos(pair.second));
			}break;
			case EObjectType::Obstacle:{
				std::shared_ptr<CObstaclebase> obst = std::dynamic_pointer_cast<CObstaclebase>(gobj);
				obst->SetRoom(GetThisPtr<CRoombase>());
				RegisterGObj(gobj, pair.second);
				obst->SetWorldPos(CoordToWorldPos(pair.second));
			}break;
			case EObjectType::Pickup:{
				std::shared_ptr<CPickup> pickup = std::dynamic_pointer_cast<CPickup>(gobj);
				RegisterGObj(gobj, pair.second);
				pickup->SetWorldPos(CoordToWorldPos(pair.second));
			}break;
			default:
				break;
			}

		}
		mbIsRoomWin = false;
	}
	else
	{
		//장애물
		for (std::pair<int, int> pair : mObstacleData)
		{
			FVector2 Coord = CChapter::Hash2Coord(pair.second);
			if (mRedFlag != FVector2(-1, -1))
			{
				if (Coord >= mRedFlag)
				{
					//LOG_DEBUG("객체 생성 취소 | 좌표가 잘못되었습니다.: ", pair.second);
					continue;
				}
			}

			std::shared_ptr<CGameObject> gobj = CGameClassContainer::GetInst()->Instantiate(pair.first, Coord, false, chapter->GetLevel()).lock();
			if (!gobj)
			{
				//LOG_DEBUG("객체 생성 실패 | 객체 ID: ", pair.first);
				continue;
			}

			std::shared_ptr<CObstaclebase> obst = std::dynamic_pointer_cast<CObstaclebase>(gobj);
			obst->SetRoom(GetThisPtr<CRoombase>());
			RegisterGObj(gobj, Coord);
			obst->SetWorldPos(CoordToWorldPos(Coord));
		}

		//몬스터
		for (std::pair<int, int> pair : mMonsterData)
		{
			FVector2 Coord = CChapter::Hash2Coord(pair.second);
			if (mRedFlag != FVector2(-1, -1))
			{
				if (Coord >= mRedFlag)
				{
					//LOG_DEBUG("객체 생성 취소 | 좌표가 잘못되었습니다.: ", pair.second);
					continue;
				}
			}

			std::shared_ptr<CGameObject> gobj = CGameClassContainer::GetInst()->Instantiate(pair.first, Coord, false, chapter->GetLevel()).lock();
			if (!gobj)
			{
				//LOG_DEBUG("객체 생성 실패 | 객체 ID: ", pair.first);
				continue;
			}

			std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(gobj);
			unit->SetRoom(GetThisPtr<CRoombase>());
			RegisterGObj(gobj, Coord);
			unit->SetWorldPos(CoordToWorldPos(Coord));
		}

		//픽업
		for (std::pair<int, int> pair : mPickupData)
		{
			FVector2 Coord = CChapter::Hash2Coord(pair.second);
			if (mRedFlag != FVector2(-1, -1))
			{
				if (Coord >= mRedFlag)
				{
					//LOG_DEBUG("객체 생성 취소 | 좌표가 잘못되었습니다.: ", pair.second);
					continue;
				}
			}

			std::shared_ptr<CGameObject> gobj = CGameClassContainer::GetInst()->Instantiate(pair.first, Coord, false, chapter->GetLevel()).lock();
			if (!gobj)
			{
				//LOG_DEBUG("객체 생성 실패 | 객체 ID: ", pair.first);
				continue;
			}

			std::shared_ptr<CPickup> pickup = std::dynamic_pointer_cast<CPickup>(gobj);
			RegisterGObj(gobj, Coord);
			pickup->SetWorldPos(CoordToWorldPos(Coord));
		}
	}
}

void CRoombase::RegisterGObj(const std::weak_ptr<class CGameObject>& GObj, const FVector2& Coord)
{
	auto obj = GObj.lock();
	if (!obj)
		return;

	switch (obj->GetObjType())
	{
	case EObjectType::PlayerCharacter:
		break;
	case EObjectType::Monster:
		mMonsterList.push_back(obj);
		break;
	case EObjectType::Obstacle:
		mObstacleGridMap[CChapter::Coord2Hash(Coord)] = std::dynamic_pointer_cast<CObstaclebase>(obj)->GetObstacleType();
		mObstacleList.push_back(obj);
		break;
	case EObjectType::Pickup:
		mPickupList.push_back(obj);
		break;
	}
}

void CRoombase::DisregisterGObj(const std::weak_ptr<class CGameObject>& GObj, const FVector2& Coord)
{
	auto obj = GObj.lock();
	if (!obj)
		return;

	//Disregister 함수가 호출되는 부분들을 다시 확인해보자 | 분명 이상한곳에서 호출하고 있을것이 뻔하다
	//그리고 호출 부분에서 뭘 하고 있는지도 중요함

	switch (obj->GetObjType())
	{
	case EObjectType::PlayerCharacter:
		break;
	case EObjectType::Monster: {
		std::list<std::weak_ptr<CGameObject>>::iterator iter = mMonsterList.begin();
		std::list<std::weak_ptr<CGameObject>>::iterator iterEnd = mMonsterList.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->lock()->GetID() == obj->GetID())
			{
				mMonsterList.erase(iter);
				return;
			}
		}
	} break;
	case EObjectType::Obstacle: {
		std::list<std::weak_ptr<CGameObject>>::iterator iter = mObstacleList.begin();
		std::list<std::weak_ptr<CGameObject>>::iterator iterEnd = mObstacleList.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->lock()->GetID() == obj->GetID())
			{
				mObstacleList.erase(iter);
				mObstacleGridMap[CChapter::Coord2Hash(Coord)] = EObstacleType::None;
				return;
			}
		}
	}break;
	case EObjectType::Pickup: {
		std::list<std::weak_ptr<CGameObject>>::iterator iter = mPickupList.begin();
		std::list<std::weak_ptr<CGameObject>>::iterator iterEnd = mPickupList.end();
		for (; iter != iterEnd; ++iter)
		{
			if (iter->lock()->GetID() == obj->GetID())
			{
				mPickupList.erase(iter);
				return;
			}
		}
	}break;
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
	//FVector2 cell = mRoomCellSize;
	//cell.y -= 10.f; // << 왜 Y만 10 빼야 하지 높이를 내가 잘 못 맞춰놨나? 
	FVector2 cal = ((Coord - mRoomCellMax / 2) * mRoomCellSize); //벽 크기가 50
	cal += mRoomCellSize / 2;
	return GetWorldPos() + FVector3(cal.x, cal.y, 1);
}

const FVector2 CRoombase::WorldPosToCoord(FVector3 WorldPos)
{
	FVector3 pos = WorldPos - GetWorldPos();
	FVector2 result = FVector2(pos.x, pos.y) - mRoomCellSize / 2;
	result /= mRoomCellSize;
	result += mRoomCellMax / 2;
	if (result.x < 0)
		result.x = round(-result.x);
	else
		result.x = round(result.x);

	if (result.y < 0)
		result.y = round(-result.y);
	else
		result.y = round(result.y);

	return result;
}

const FVector2 CRoombase::GetPlayerCoordInGrid()
{
	std::shared_ptr<CChapter> chpter = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	if (!chpter)
		return -FVector2::One;
	std::shared_ptr<CUnitbase> pUnit = std::dynamic_pointer_cast<CUnitbase>(chpter->GetPlayerCharacter().lock());
	if (!pUnit)
		return -FVector2::One;

	return WorldPosToCoord(pUnit->GetWorldPos());
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

	FVector2 PlayerCoord = WorldPosToCoord(pUnit->GetWorldPos());
	FVector2 FromCoord = WorldPosToCoord(FromWorldPos);
	if (!CheckNearCell(FromCoord) || !CheckNearCell(PlayerCoord) || !CheckCell(PlayerCoord))
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
		if (EObstacleType::None == mObstacleGridMap[hash])
			return true;
	}
	return false;
}

bool CRoombase::CheckCell(FVector2 Coord)
{
	if (Coord.x < 0 || Coord.y < 0 || Coord.x >= mRoomCellMax.x || Coord.y >= mRoomCellMax.y)
		return false;

	int hash = CChapter::Coord2Hash(Coord);
	if (EObstacleType::None == mObstacleGridMap[hash])
		return true;

	return false;
}

void CRoombase::ConnectRoom(std::weak_ptr<CRoombase> Room)
{
	if (Room.expired())
	{
		//LOG_DEBUG("Room이 빈 객체입니다.");
		return;
	}

	mNearRooms.push_back(std::make_pair(Room.lock()->mCoord - mCoord, Room));

}

bool CRoombase::HasNearRoom(FVector2 Dir)
{
	for (std::pair<FVector2, std::weak_ptr<CRoombase>> pair : mNearRooms)
	{
		if (Dir == pair.first)
			return true;
	}
	return false;
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