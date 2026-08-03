#include "Chapter.h"
#include "../Rooms.h"

#include "Manager/GameRuleManager.h"

#include "LogManager.h"

#include "GameSystemActor.h"

FVector2 CChapter::FourDirections[4] =
{
	FVector2(1, 0),
	FVector2(-1, 0),
	FVector2(0, 1),
	FVector2(0, -1)
};
FVector2 CChapter::EightDirections[8] =
{
	FVector2(2, -1),	//우측 하단
	FVector2(2, 1),		//우측 상단
	FVector2(-2, -1),	//좌측 상단
	FVector2(-2, 1),	//좌측 상단
	FVector2(1, 2),		//상단 우측
	FVector2(-1, 2),	//상단 좌측
	FVector2(1, -2),	//하단 우측
	FVector2(-1, -2)	//하단 좌측
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

	mChapterManagementActor = CreateActor<CChapterSystemActor>("GSA");

	mRoomRowMax = 10;
	mRoomColMax = 10;
	GenerateRoom();
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
void CChapter::GenerateRoom()
{
	FVector2 StartCoord(static_cast<float>(mRoomRowMax / 2), static_cast<float>(mRoomColMax / 2));
	std::weak_ptr<CRoombase> generatedRoom = CreateRoom<class CDefaultRoom>("Start", StartCoord);
	std::shared_ptr<CRoombase> room = generatedRoom.lock();
	if (!room)
	{
		LOG_DEBUG("방 생성 실패");
		return;
	}

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
bool CChapter::ReturnGObj(std::weak_ptr<CGameObject> Obj)
{
	if (Obj.expired())
		return false;

	EObjectType t =  Obj.lock()->GetObjType();
	if (EObjectType::Monster == t)
	{
		std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(Obj.lock());
		mUnits[unit->GetID()].first = false;
		return true;
	}
	else if (EObjectType::Pickup == t)
	{
		
	}

	return false;
}
const int CChapter::GetIsValidCoord(const FVector2& Coord)
{
	if (Coord.x >= mRoomRowMax || Coord.x < 0 || Coord.y >= mRoomColMax || Coord.y < 0)
		return 0;

	if (!mRoomMap[Coord2Hash(Coord)].expired())
		return 1;
	else
		return 2;
}
std::weak_ptr<CRoombase> CChapter::GetRoom(FVector2 Coord)
{
	if (Coord.x >= mRoomRowMax || Coord.x < 0 || Coord.y >= mRoomColMax || Coord.y < 0)
		return std::weak_ptr<CRoombase>();
	return mRoomMap[Coord2Hash(Coord)];
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