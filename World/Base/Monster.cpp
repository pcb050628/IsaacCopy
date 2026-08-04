#include "Monster.h"

CMonster::CMonster()
	:CUnitbase(EObjectType::Monster)
{
}

CMonster::CMonster(const CMonster& src)
	:CUnitbase(src)
{
}

CMonster::CMonster(CMonster&& src) noexcept
	:CUnitbase(std::move(src))
{
}

CMonster::~CMonster()
{
}

void CMonster::FindRouteToPlayerCharacter()
{
	//내 좌표 찾기
	//플레이어의 좌표 찾기
	//내 좌표에서 플레이어 좌표로 가는 길 찾기
	//1. 현재 위치에서 갈수있는 길들을 모두 펴놓는다.(4방향)
	//		네 방향은 각각 코스트를 지닌다.(0으로 시작)
	//2. 처음에는 가장 가까운 방향으로 진행한다.
	//3. 진행중 진행 방향의 코스트보다 다른 방향의 코스트가 낮은 경우 해당 방향으로 바꿔서 진행한다.
	//4. 반복
	//코스트 계산- 진행한 칸당 1점, 남은 거리(그냥 목표좌표 - 진행좌표 의 거리), 만약 도달할 수 없다면 해당 길을 폐기
	//실행함수 -> 길 진행함수 로 4방향 진행하고 누적점수는 실행함수에서 보내기
	//FindRoute(Fvetor2 dir, int targetIdx, std::vector<int>& routePoint);
}

bool CMonster::CanGetToPlayerCharacter()
{
	return false;
}
