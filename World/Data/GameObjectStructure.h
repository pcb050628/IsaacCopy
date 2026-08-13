#pragma once
#include "Math/Vector2.h"

struct FFrameData
{
	FVector2 Start;
	FVector2 Size;
};

struct FAnimationData
{
	std::string Name;
	std::string TextureName;
	std::string TexturePath;
	std::vector<FFrameData> Frames;
};

struct FRoomObjectData
{
	int ID;
	FVector2 Coord;
};

struct FItemData
{
	//어떤 아이템인지(아이디)
	int ID;
	//더 알게 뭐가 있더라
	//에너지 얼마나 있었는지
	int ChargedEnerge;
	//몇스택인지(스택을 쌓는 아이템이 몇가지 있음
	int Stack;
};

struct FPlayerData
{
	//어떤캐릭터인지(아이디)
	int ID;
	//체력(정수)
	int HPCount;
	//방위치(좌표) , 방 내의 위치(좌표)
	FVector2 ChapterCoord;
	FVector2 RoomCoord;
	//가지고 있는 아이템들(배열)
	std::vector<FItemData> Items;
};

//이거 필요한지 모르겠다. 방에 합치면 안되나? 따로 둬야하는 이유가 있나? 
//특수방들 때문에 필요한거같기도 하고
//문의 순서를 알면 이거 필요 없을거같은데
//항상 좌상단 기준으로 시계방향으로 저장하면
//L모양은 어떻게 하지
//
//struct FDoorData 
//{
//	//열렸는지
//};
//
struct FRoomData
{
	//어떤 방인지(아이디)
	int ID;
	//챕터 내 방의 위치 | 초기는 -1, 이 경우에는 방에 위치를 할당하지않고 자동으로 위치를 정해줌
	FVector2 Coord = -FVector2::One;
	//클리어 상태
	bool Clear;
	//문들의 상태 / 항상 좌상단에서 시계방향 / L모양의 경우 TL->RT->TR->R->RB...
	std::vector<bool> Doors;
	//내부 오브젝트들의 위치와 아이디 FRoomObjectData
	//초기 상태 내부 오브젝트들의 위치와 아이디
	//일단은 사용안하는 중
	//필요해지면 CRoomGData 클래스의 함수들 수정해주기
	std::vector<FRoomObjectData> InitObjs; 
};

struct FChapterData
{
	//챕터 레벨
	int ChapterLevel;
	//저주(나중에 만들고 추가하기)
	//방들
	std::vector<FRoomData> Rooms;

	//일단 더 기억나느게 없으니 이대로 진행
};

struct FRunData
{
	//시드uint
	unsigned int Seed;
	//챕터(정수
	int CurrentChapterLevel;
	//방들
	std::vector<FChapterData> Chapters;
	//플레이어 정보(픽업, 캐릭터, 아이템, 
	FPlayerData Player;
};

//    //정리 결과
//    //1. 게임 관련
//    //-방, 게임 정보, 플레이어 정보(저장할 데이터가 거의 없어서 합치려고 넣었음
//    //2. 플레이어(캐릭터) 관련
//    //-캐릭터 정보, 아이템(플레이어에게 없으면 필요 없어서 플레이어에 넣었음