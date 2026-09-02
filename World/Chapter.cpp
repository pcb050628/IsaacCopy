#include "Chapter.h"
#include "../Rooms.h"

#include "RenderManager.h"
#include "World/Input.h"

#include "Manager/GameClassContainer.h"
#include "Manager/GameRuleManager.h"

#include "Asset/AssetManager.h"

#include "Data/GameDataManager.h"
#include "Data/GameObjectStructure.h"
#include "Data/RoomGData.h"

#include "LogManager.h"

#include "Base/Character.h"
#include "GameSystemActor.h"
#include "Door.h"

#include "World/ColliderBox2D.h"

const float CChapter::WallSize = 50.f;
const FVector2 CChapter::FourDirections[4] =
{
	FVector2(0, 1),
	FVector2(0, -1),
	FVector2(-1, 0),
	FVector2(1, 0),
};
const FVector2 CChapter::RoomWorldSize = FVector2(1300.f, 700.f);
const FVector2 CChapter::EightDirections[8] =
{
	FVector2(-1, 2),	//상단 좌측
	FVector2(1, 2),		//상단 우측
	FVector2(2, 1),		//우측 상단
	FVector2(2, -1),	//우측 하단
	FVector2(1, -2),	//하단 우측
	FVector2(-1, -2),	//하단 좌측
	FVector2(-2, -1),	//좌측 하단
	FVector2(-2, 1),	//좌측 상단
};

CChapter::CChapter()
{
}

CChapter::~CChapter()
{
}

bool CChapter::Init()
{
	CWorld::Init();
	//랜덤 초기화
	CGameRuleManager::GetInst()->SetRandomSeed();
	//방 생성하기
	//최소 (층 * 5      : 1 * 5 =       5 | 2 * 5 =       10)
	//최대 (층 * 5.5 + 3: 1 * 5.5 + 3 = 8 | 2 * 5.5 + 3 = 14)

	mChapterLevel = 1;
	CRenderManager::GetInst()->SetState("AlphaBlend");
	mChapterManagementActor = CreateActor<CChapterSystemActor>("GSA");
	mChapterManagementActor.lock()->OnMoveEnd(this, &CChapter::SettingFocus);

	GenerateWallAndDoor();

	mRoomMapRowMax = 10;
	mRoomMapColMax = 10;
	GenerateNormalRoom();
	GenerateSpecialRoom();
	//다시 적는 생성 규칙
	//1. 시작 방 생성
	//2. 시작 방으로부터 4방향으로 진행(생성)
	//3. 진행 방향으로 방 생성 후 연결
	//3-1. 만약 진행방향에 방이 이미 존재한다면 해당 방향으로 이어서 진행
	//4. 진행 방향으로 생성 한 방에서 다시 4방향으로 진행
	//5. 진행 중 생성된 방의 개수가 최대값보다 크거나 같다면 바로 반환
	//6. 시작 방에서 시작한 4방향을 다 완료했을때 최소 값보다 방이 적으면 다시 진행

	return true;
}
void CChapter::Update(float DeltaTime)
{
	CWorld::Update(DeltaTime);
	//1. 현재 포커스 중인 방만 업데이트
	//2. 다른 방들은 어짜피 disable 상태니까 그냥 전부 업데이트
	//
	//		굳이 포커스 중인 방을 이미 알고 있는데 다른 방들을 업데이트 해야하는 이유가 있나?
	//		
	// 
	// 
	//필수. GameTimeScale 을 만들어서 DeltaTime 이랑 같이 보내기
	//		GameTimeScale 의 위치는 GameRuleManager
	//
	//GameRuleManager에 대해서
	//들어가는 것들은 확률들과 기타 규칙들
	//예를 들면 잠긴문의 여는 조건이 열쇠일때 폭파로 열 수 있게 하거나
	//이런것들은 한번정해지고 바뀔수도 있긴하다만
	//챕터가 변함에 따라서 변하는 규칙들은 아니기 때문에
	//챕터에 귀속시킬 필요는 없을듯?
	//게임 초기화와 관련된 것들을 포함해도 되겠다.
	//규칙을 들을 다 가지고 있으니 외부 객체들이 상황마다 규칙을 확인하기 위해서 호출할테니
	//어짜피 스태틱 클래스가 될테고 / 아니면 객체 내부 규칙을 바꿔버릴까? -
	//		1. 내부에서 보관=>상황마다 외부에서 호출함, 위의 예시에 해당하는 상황은 그렇게 빈번하지 않아서 괜찮긴하지만 생각해보니 이런 상황이 많이 없음
	//							많이 있는 상황들은 내부에 있는 규칙이 아니라 외부에 추가로 할당하는 함수같은 개념이라 문제없을듯
	//		2. 외부에 위임=>외부에서 호출할 일이 거의 없음, 위의 예시에 해당하는 상황에서도 스스로 판단할 수 있음
	//	
	//		그래서 2가 안될 이유는 뭘까
	//		짧게 생각한 결과 없는 것ㄴ 같으니
	//		2로 만들자
	//게임의 초기화는 한번이면 되는데 챕터(월드, 레벨) 생성에 맞춰가며 하는 것 보다는 
	//다른 매니저 객체와 같이 초기화 하고 챕터를 생성하는 방향이 더 좋아보인다.
	//

	//UI에는 TimeScale 을 적용하지 않음
	//UI는 방이 아닌 챕터에서 관리할 것이고
	//키 입력으로 On/Off 가능하게 할 것임
	//UI 상태에 따라서 Input을 바꿈
}
void CChapter::GenerateWallAndDoor()
{
	for (int i = 0; i < 12; ++i)
	{
		auto actor = CreateActor<CActor>("Wall").lock();
		auto col = actor->CreateComponent<CColliderBox2D>("Root").lock();
		col->SetDebugDraw(true);
		col->SetCollisionProfile("Wall");
		actor->SetEnable(false);
		actor->SetRenderEnable(false);
		mWalls.push_back(actor);
	}

	for (int i = 0; i < 8; ++i)
	{
		auto door = CreateActor<CDoor>("Door");
		door.lock()->SetEnable(false);
		door.lock()->SetRenderEnable(false);
		mDoorsDeactive.insert(std::make_pair(door.lock()->GetID(), door));
	}
}
void CChapter::GenerateNormalRoom()
{
	FVector2 StartCoord(static_cast<float>(mRoomMapRowMax / 2), static_cast<float>(mRoomMapColMax / 2));
	std::weak_ptr<CRoombase> generatedRoom = CreateRoom<class CDefaultRoom>("Start", StartCoord);
	std::shared_ptr<CRoombase> room = generatedRoom.lock();
	if (!room)
	{
		LOG_DEBUG("방 생성 실패");
		return;
	}
	RegisterRoom(room);

	int min = mChapterLevel * 5;
	int max = static_cast<int>(mChapterLevel * 5.5f + 3);
	int CurrentSize = 1;
	while (CurrentSize < min)
	{
		room->GenerateRoom(FourDirections[0], min, max, CurrentSize);
		room->GenerateRoom(FourDirections[1], min, max, CurrentSize);
		room->GenerateRoom(FourDirections[2], min, max, CurrentSize);
		room->GenerateRoom(FourDirections[3], min, max, CurrentSize);
	}
}
void CChapter::GenerateSpecialRoom()
{
	bool treasure = false;
	bool shop = false;
	bool boss = false;
	for (std::pair<int, std::weak_ptr<CRoombase>> pair : mRoomMap)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (pair.second.expired())
				continue;
			FVector2 center = Hash2Coord(pair.first);
			FVector2 direction = FourDirections[i];
			bool check = pair.second.lock()->HasNearRoom(direction);
			if (check)
				continue;

			center += direction;

			int count = 0;
			count = mRoomMap[Coord2Hash(center + FourDirections[0])].expired() ? count : count + 1;
			count = mRoomMap[Coord2Hash(center + FourDirections[1])].expired() ? count : count + 1;
			count = mRoomMap[Coord2Hash(center + FourDirections[2])].expired() ? count : count + 1;
			count = mRoomMap[Coord2Hash(center + FourDirections[3])].expired() ? count : count + 1;

			if (count < 2)
			{
				std::weak_ptr<CRoombase> generatedRoom = std::dynamic_pointer_cast<CRoombase>(CGameClassContainer::GetInst()->Instantiate(11, center).lock()); //생성 후 내부에서 방 연결하기
				std::shared_ptr<CRoombase> room = generatedRoom.lock();
				if (!room)
					return;

				std::vector<std::pair<int, FVector2>> data;
				data.push_back(std::make_pair(401, FVector2(6, 3)));
				if (!room->SetInitRoom(data))
				{
					LOG_DEBUG("방 생성 실패");
				}
				RegisterRoom(room);
				room->AdjustRoomPos();
				return;
			}
		}
	}
}
void CChapter::InitialSetting()
{
	//현재 포커스 중인 방으로 벽 이동시키기
	//기본 방 크기 기준으로만 계산중
	auto room = mRoomMap[mFocusedRoomHash].lock();
	ERoomShape shape = room->GetRoomShape();
	FVector3 center = room->GetWorldPos();
	FVector2 size = room->GetRoomSize();
	int wallidx = 0;
	for (int i = 0; i < 4; ++i)
	{
		FVector2 pos = FourDirections[i];
		pos.x *= 575.f;
		pos.y *= 290.f;
		for (int j = -1; j < 2; ++j)
		{
			if (0 == j)
				continue;
			FVector2 wallOffset = pos;
			if (0 == FourDirections[i].x)
				wallOffset.x += j * ((RoomWorldSize.x / 4) - 25);
			else if (0 == FourDirections[i].y)
				wallOffset.y += j * ((RoomWorldSize.y / 4) - 25);

			auto wall = mWalls[wallidx++].lock();
			wall->SetWorldPos(center + FVector3(wallOffset.x, wallOffset.y, 0));
			wall->SetEnable(true);
			wall->SetRenderEnable(true);
		}
	}

	std::static_pointer_cast<CColliderBox2D>(mWalls[0].lock()->GetRootComponent().lock())->SetBoxSize(size.x / 2, WallSize);
	std::static_pointer_cast<CColliderBox2D>(mWalls[1].lock()->GetRootComponent().lock())->SetBoxSize(size.x / 2, WallSize);

	std::static_pointer_cast<CColliderBox2D>(mWalls[2].lock()->GetRootComponent().lock())->SetBoxSize(size.x / 2, WallSize);
	std::static_pointer_cast<CColliderBox2D>(mWalls[3].lock()->GetRootComponent().lock())->SetBoxSize(size.x / 2, WallSize);

	std::static_pointer_cast<CColliderBox2D>(mWalls[4].lock()->GetRootComponent().lock())->SetBoxSize(WallSize, size.y / 2);
	std::static_pointer_cast<CColliderBox2D>(mWalls[5].lock()->GetRootComponent().lock())->SetBoxSize(WallSize, size.y / 2);

	std::static_pointer_cast<CColliderBox2D>(mWalls[6].lock()->GetRootComponent().lock())->SetBoxSize(WallSize, size.y / 2);
	std::static_pointer_cast<CColliderBox2D>(mWalls[7].lock()->GetRootComponent().lock())->SetBoxSize(WallSize, size.y / 2);

	//문에 방 이동 연결해야하는데
	//방 이동은? 챕터에서 해야겠지
	//방을 어떻게 이동하느냐
	//1. 현재 방 즉시 Enable false 해주고
	//2. 다음방으로 카메라 이동(set 말고 add로 이동 | 이걸 위해서 Rigidbody 달아주기 | addforce 로 가속주고 limit 달아서 너무 빨라지지않게)
	//3. 플레이어 위치 셋 해주고 몇 초 후 enable true
	room->EnterRoom();
	LOG_DEBUG("현재 위치: ", mFocusedRoomHash);

	mChapterManagementActor.lock()->SetWorldPos(center);
	mPlayerCharacter.lock()->SetEnable(true); //중앙 이동 말고 내부 함수 따로 작성해서 방 모양에 따라서 이동하게 만드릭
	//추가로 ChatperSystemActor 내부에 플레이어 따라다니는 함수 작성하기 | 방이 커졌을때 필요함
}
void CChapter::SettingFocus() //지금은 벽만 설정하지만 이 함수를 포커스 이동시 초기 설정 함수로 만들기(벽, 문 모두 이동 설정하기)
{
	mPlayerCharacter.lock()->UnsetRoom();
	mRoomMap[mPrevRoomHash].lock()->ExitRoom();
	InitialSetting();
	//플레이어 위치 옮겨주기
	//그리고 타이머 넣어서 enable 넣어주기
	SetPlayerPos();
	//mRoomEnterTimer = CTimeManager::SetTimer(0.1f, false, this, &CChapter::SetPlayerPos);
}
void CChapter::SetPlayerPos()
{
	FVector2 dir = Hash2Coord(mFocusedRoomHash) - Hash2Coord(mPrevRoomHash);
	FVector2 dist = -dir * RoomWorldSize / 2;
	if (0 == dir.x)
		dist.y += dir.y * WallSize * 2;
	else
		dist.x += dir.x * WallSize * 2;
	mPlayerCharacter.lock()->SetWorldPos(mRoomMap[mFocusedRoomHash].lock()->GetWorldPos() + FVector3(dist.x, dist.y, 0));
	mPlayerCharacter.lock()->SetRoom(mRoomMap[mFocusedRoomHash]);
	mInput->SetEnable(true);
	LOG_DEBUG("플레이어의 방 좌표: ", mFocusedRoomHash);
}
std::weak_ptr<class CDoor> CChapter::GetDoor(ERoomType roomType)
{
	if (mDoorsDeactive.empty())
	{
		std::weak_ptr<CDoor> door = CreateActor<CDoor>("Door");
		if (door.expired())
			return std::weak_ptr<CDoor>();

		door.lock()->SetDoorFrameType(roomType);
		mDoorsActive.insert(std::make_pair(door.lock()->GetID(), door));
		return door;
	}
	else
	{
		std::weak_ptr<CDoor> door = mDoorsDeactive.begin()->second;
		mDoorsDeactive.erase(mDoorsDeactive.begin());
		if (door.expired())
			return std::weak_ptr<CDoor>();
		door.lock()->SetDoorFrameType(roomType);
		mDoorsActive.insert(std::make_pair(door.lock()->GetID(), door));
		return door;
	}
}
bool CChapter::ReturnGObj(std::weak_ptr<CGameObject> Obj)
{
	if (Obj.expired())
		return false;

	std::shared_ptr<CGameObject> gobj = Obj.lock();
	std::shared_ptr<CRoomMember> rm = std::dynamic_pointer_cast<CRoomMember>(gobj);

	gobj->SetEnable(false);
	gobj->SetRenderEnable(false);
	const int classID = gobj->GetGClassID();
	const int ObjID = gobj->GetID();
	EObjectType t = gobj->GetObjType();
	switch (t)
	{
	case EObjectType::Room:
		break;
	case EObjectType::Door:
		mDoorsDeactive[ObjID] = mDoorsActive[ObjID];
		mDoorsActive.erase(ObjID);
		break; 
	case EObjectType::Tear: {
		mTearsDeactivate[ObjID] = mTearsActivate[ObjID];
		mTearsActivate.erase(ObjID);
	} return true;
	case EObjectType::Monster: {
		rm->UnsetRoom().lock()->DisregisterGObj(rm);
		mUnitsDeactivate[classID][ObjID] = mUnitsActivate[classID][ObjID];
		mUnitsActivate[classID].erase(ObjID);
	} return true;
	case EObjectType::Obstacle: {
		rm->UnsetRoom().lock()->DisregisterGObj(rm);
		mObstaclesDeactivate[classID][ObjID] = mObstaclesActivate[classID][ObjID];
		mObstaclesActivate[classID].erase(ObjID);
	} return true;
	case EObjectType::Pickup: { //애초에 등록을 안함 | 근데 해야됨 -> 나중에 수정하기
		rm->UnsetRoom().lock()->DisregisterGObj(rm);
		mPickupsDeactivate[classID][ObjID] = mPickupsActivate[classID][ObjID];
		mPickupsActivate[classID].erase(ObjID);
	} return true;
	}

	return false;
}
void CChapter::MoveRoom(FVector2 Dir)
{
	mChapterManagementActor.lock()->Move(Dir);
	mPrevRoomHash = mFocusedRoomHash;
	FVector2 coord = Hash2Coord(mFocusedRoomHash) + Dir;
	LOG_DEBUG("다음 좌표는: ", std::to_string(coord.x), ", ", std::to_string(coord.y));
	mFocusedRoomHash = Coord2Hash(coord);
	mInput->SetEnable(false);
	mPlayerCharacter.lock()->SetEnable(false);
	mRoomMap[mPrevRoomHash].lock()->PauseRoom();
	mRoomMap[mFocusedRoomHash].lock()->PauseRoom();
}
void CChapter::RegisterRoom(const std::shared_ptr<CRoombase>& room)
{
	if (!room)
		return;

	FVector2 Coord = room->GetCoord();
	if (mRoomMap.contains(Coord2Hash(Coord)) && !mRoomMap[Coord2Hash(Coord)].expired())
		return;

	if (-1 == mFocusedRoomHash)
	{
		mFocusedRoomHash = Coord2Hash(Coord);
		mRoomMap[mFocusedRoomHash] = room;
	}
	else
	{
		mRoomMap[Coord2Hash(Coord)] = room;
	}

	for (int i = 0; i < 4; i++)
	{
		int dest = Coord2Hash(Coord + FourDirections[i]);
		if (!mRoomMap[dest].expired())
		{
			room->ConnectRoom(mRoomMap[dest]);
			mRoomMap[dest].lock()->ConnectRoom(room);
		}
	}
	room->PauseRoom();
}

void CChapter::RegisterGObjToRoom(const std::weak_ptr<CRoomMember>& rm, const FVector2& Coord, const FVector2& targetRoomCoord)
{
	if (rm.expired() || !GetIsValidCoord(targetRoomCoord))
		return;

	std::shared_ptr<CRoombase> room = mRoomMap[Coord2Hash(targetRoomCoord)].lock();
	if (!room)
		return;

	room->RegisterGObj(rm, Coord);
	std::weak_ptr<CRoombase> prevRoom = rm.lock()->UnsetRoom();
	if (!prevRoom.expired())
	{
		prevRoom.lock()->DisregisterGObj(rm);
	}
	rm.lock()->SetRoom(room);
}

void CChapter::RegisterCharacter(const int ID)
{
	FVector2 startCoord(6, 3);
	if (-1 != mFocusedRoomHash)
		startCoord = mRoomMap[mFocusedRoomHash].lock()->GetRoomCellMax() / 2;

	std::shared_ptr<CCharacter> chara = std::dynamic_pointer_cast<CCharacter>(CGameClassContainer::GetInst()->Instantiate(ID, startCoord).lock());
	
	assert(chara && "캐릭터 생성 실패: ", ID);

	CGameRuleManager::GetInst()->RegisterPlayerHeartContainer(chara->GetID());
}

const int CChapter::GetIsValidCoord(const FVector2& Coord)
{
	if (Coord.x >= mRoomMapRowMax || Coord.x < 0 || Coord.y >= mRoomMapColMax || Coord.y < 0)
		return 0;

	if (!mRoomMap[Coord2Hash(Coord)].expired())
		return 1;
	else
		return 2;
}
std::weak_ptr<CRoombase> CChapter::GetRoom(FVector2 Coord)
{
	if (Coord.x >= mRoomMapRowMax || Coord.x < 0 || Coord.y >= mRoomMapColMax || Coord.y < 0)
		return std::weak_ptr<CRoombase>();
	return mRoomMap[Coord2Hash(Coord)];
}
std::weak_ptr<CRoombase> CChapter::GetFocusedRoom()
{
	return mRoomMap[mFocusedRoomHash];
}
void CChapter::RenderTitleWithQuato(const TCHAR* title, const TCHAR* quato)
{
	mChapterManagementActor.lock()->DrawTitleWithQuato(title, quato, 5.f);
}
//
//첫 생성 함수는 일반함수로 모든 방향을 호출하고 두번째부터는 재귀함수 랜덤방향으로 이어가기 만약 이미 방이 있는 방향이 나온 경우 해당 방을 지나서 그 방향으로 쭉 이동 후 계속하기
//만약 모든 생성을 마쳤는데 방의 개수가 최소치를 맞추지 못했다면 다시 실행
//#
//갈 수 있는 방향들 만들어두고
//모든 방향 한번씩 단 갈 수 있는 경우에만 방 개수가 이미 최대거나 하면 못감
//좌표를 가지니까 좌표로 기존에 존재하는 방 찾고 생성되자마자 이어주기
//
// 1. 층에 비례해 최대 개수생성 ( 층 * 3.5: 1 * 3.5 = 3 x 3: 2 * 3.5 = 7 x 7): 방의 개수는 이 최대 수를 넘으면 안됨
// 2. 시작점 찍기 (시작위치 저장)
// 3. 시작점으로 부터 4방면 모두 뻗어나가기
// 4. 뻗어나가다가 확률적으로 그만두거나 꺾기
// 4-1. 뻗어나가다가 확률적으로 큰방으로 만들기 최대 층*0.7 만큼(두칸, 두칸+1(ㄴ), 4칸(ㅁ) 모양)
// 5. 4방면 모두 생성을 마친 후 특수 방 생성하기
// 5-1. 황금방, 상점, 보스방, 일급 비밀방 등 특수방들은 비밀방을 제외한 인접한 방이 하나여야만 한다
// 5-2. 일반 비밀방은 인접한 방이 3개 이상이어야 함
// 6. 일반방 채우기(몬스터, 장애물 등을 미리 찍어둔 리소스에서 가져가기)
// 6-0. 방은 미리 찍어두로 리소스로 관리할 것임(방의 정보는 오브젝트들의 좌표와 몬스터의 종류 및 좌표)
// 7. 특수방 채우기
// 7-1. 상점, 황금방은 랜덤으로 아이템만 채움 (아이템은 아이템 풀에서 가져가기
// 7-2. 보스방은 랜덤으로 보스몬스터만 채움 (아이템은 아이템 풀에서 가져가기
// 7-3. 비밀방은 리소스에서 골라가기
//

//
// 방이 한칸일때는 좌표를 1:1로 매칭해도 문제가 없는데
// 방이 여러방향을 넓어지면 좌표를 매칭하기 불편하다.
// 3,3 의 L모양 방이 우측으로 진행하면 어떻게 해야할까
// 모양마다 중심점이 있다고 생각하고 중심점으로 부터 진행한다.
// L모양 방의 겨우 중심점이 우하단이므로
// 우하단(그냥 우측)으로 진행하면 4, 3
// 우상단(한칸 위 우측)으로 진행하면 2, 2 가 되겠다.
// 이렇듯 방마다 진행 방식이 다르니 방 생성 함수는 방 클래스로 위임하는것이 맞겠다.
// 
//