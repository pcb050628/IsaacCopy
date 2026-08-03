#pragma once
#include "Math/Vector2.h"

struct FRoomObjectData
{
	int ID;
	FVector2 Coord;
};

struct FItemData
{

};

struct FPlayerData
{
	//어떤캐릭터인지(아이디)
	//체력(정수)
	//방위치(좌표) , 방 내의 위치(좌표)
	//가지고 있는 아이템들(배열)
};

struct FDoorData
{
	//열렸는지
};

struct FRoomData
{
	//어떤 방인지(아이디)
	int ID;
	//챕터 내 방의 위치
	FVector2 Coord;
	//클리어 상태
	bool Clear;
	//내부 오브젝트들의 위치와 아이디 FRoomObjectData
	std::vector<FRoomObjectData> CurrentObjs;
	//초기 상태 내부 오브젝트들의 위치와 아이디
	//일단은 사용안하는 중
	//필요해지면 CRoomGData 클래스의 함수들 수정해주기
	std::vector<FRoomObjectData> InitObjs; 
};

struct FRunData
{
	//챕터(정수
	//방들
	//플레이어 정보(픽업, 캐릭터, 아이템, 
};

//    //정리 결과
//    //1. 게임 관련
//    //-방, 게임 정보, 플레이어 정보(저장할 데이터가 거의 없어서 합치려고 넣었음
//    //2. 플레이어(캐릭터) 관련
//    //-캐릭터 정보, 아이템(플레이어에게 없으면 필요 없어서 플레이어에 넣었음