#include "GameDataManager.h"

CGameDataManager* CGameDataManager::mInstance;

CGameDataManager::CGameDataManager()
{

}

CGameDataManager::~CGameDataManager()
{

}

bool CGameDataManager::Init()
{
    return true;
}

std::weak_ptr<CGameData> CGameDataManager::FindData(const std::string& Name)
{
    if(mMap.find(Name) == mMap.end())
        return std::weak_ptr<CGameData>();
    return mMap[Name];
}

//bool GameDataManager::LoadDataFile(const TCHAR* FileName)
//{
//    //저장해야하는 데이터의 종류는 얼마나 될까
//    //방, 몬스터, 캐릭터, 픽업, 아이템, 플레이어 정보, 게임 정보
//
//    //데이터들은 어떻게 다른가
//    //방: 아이디(어떤 방인지), 좌표(챕터내의 좌표), 클리어 여부, 오브젝트(아이디와 좌표 / 포괄적으로 묶는 이유는 어짜피 아이디로 다 생성 가능함)
//    //몬스터: 없는거같은데
//    //캐릭터: 아이디(어떤 캐릭터인지), 체력, 아이템, 좌표(챕터의 방위치 및 방 내부의 격자 위치 두개 다 필요), 
//    //픽업: 없는듯
//    //아이템: 아이디(어떤 아이템인지), 그냥 들고있는 데이터 그대로 다 넣어야할거같으넫
//    //        액티브는 제외하고 패시브 기준으로 생각하면
//    //        1. 스탯 / 눈물의 영향을 주는 아이템: 저장할것 없음
//    //        2. 정보 누적이 필요한 아이템: 누적 정도를 저장해야함 혹시 모르니까 그냥 float 으로 stack 용 변수 하나 만들어 두면 될듯
//    //        3. 
//    //플레이어 정보: 픽업 상태, 말고 뭑 ㅏ있나?
//    //게임 정보: 현재 시드, 현재 챕터 및 챕터 상태(방을 얼마나 클리어 했는지, 보스는 잡았는지 등), 드랍률, 아이템 가중치
//
//    //정리 결과
//    //1. 게임 관련
//    //-방, 게임 정보, 플레이어 정보(저장할 데이터가 거의 없어서 합치려고 넣었음
//    //2. 플레이어(캐릭터) 관련
//    //-캐릭터 정보, 아이템(플레이어에게 없으면 필요 없어서 플레이어에 넣었음
//
//    //그래서 이 데이터들은 어떻게 얼마나 다른가
//    //아니 근데 그게 중요한가
//    //어짜피 객체가 달라서 다 다르게 가져와야하는데
//    //템플릿으로 만들 수 밖에
//    return false;
//}
