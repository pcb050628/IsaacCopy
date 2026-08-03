#pragma once
#include "GameObject.h"
#include "World\WorldManager.h"

#define ROOM_GRID_SIZE 100
#define ROOM_GRID_HALF ROOM_GRID_SIZE / 2

enum class ERoomShape
{
    Normal,
    Vertical,
    Horizontal,
    LShape,
    Double,
    End,
};

enum class ERoomType
{
    Normal,
    Boss,
    Shop,
    Treasure,
    Start,
    Angel,
    Devil,
    Secret,
    End,
};

class CRoombase :
    public CGameObject
{
public:
    CRoombase(ERoomType Type, ERoomShape Shape);
    CRoombase(const CRoombase& src);
    CRoombase(CRoombase&& src) noexcept;
    virtual ~CRoombase();

public:
	virtual bool Init(); 
    virtual void Update(float DeltaTime) = 0; //클리어 여부 검사 / 방 마다 다르지만 기본으로는 모든 적이 비활성화 상태시 클리어
	virtual void Destroy() = 0;

protected:
    ERoomShape mShape;
    ERoomType mRoomType;
    //현재 챕터에서 방의 좌표
    FVector2 mCoord;
    //인접한 방들 / 여러 모양의 방에서 사용가능하게 벡터방향으로 접근(좌상단은 -1, 1 / 좌는 -1, 0) 나올 수 있는 문의 개수는 최대 8개
    //              문의 위치 계산기준은 항상 아래쪽 0, 위 1이다
    //해당 멤버변수는 초기화 단계에서 설정해줘야함
    std::list<std::pair<FVector2, std::weak_ptr<CRoombase>>> mNearRooms;

    //아래의 객체들은 모두 월드에서 생성
    //CreateActor를 사용하지 않고 각각 객체를 따로 생성함
    //CreateDoor, CreateObstacle, CreateMonster, CreatePickup
    //이렇게 하는 이유는 월드에서 객체들을 등록해둬야 관리하기에 편함
    //몬스터가 죽었다고 해서 메모리를 해제하는게 아니라 비활성화 시키기 때문
    //그래도 처음 생성 단계에서는 서로 다른 방끼리 하나의 객체를 참조하지 않게 방마다 각각 객체들을 생성할것임
    //만약 최적화가 필요하다고 생각되는 정도로 느려지면 모든 방에서 최소한의 객체를 같이 참조하도록 변경하기
    std::list<std::weak_ptr<class CDoor>> mDoors;           //문 / 방 생성이 끝난 후 생성
    std::list<std::weak_ptr<class CObstaclebase>> mObstacle;    //장애물(돌, 불 등의 체력이 없고 파괴가능한 객체들)
	std::list<std::weak_ptr<class CUnitbase>> mUnits;       //유닛
    //벽이 필요한데, 벽의 경우 나올 수 있는 모양들이 몇가지 없어고 비활성화될 일도 없어서 월드에 올려두고 돌려쓰는 형식으로 쓸 것
    //따라서 벽을 세팅하는 함수를 작성하거나 Reset 함수 내부에 작성해야한다
    
    FVector2 mRedFlag = FVector2(-1, -1); //넘을 수 없는 위치 / 사용하는 방은 L모양 하나뿐이지만 일단 만들어둠
    std::vector<std::pair<int, FVector2>> mInitInfo;

    bool mbIsRoomWin = false;
    //보상 / 해당 클래스는 아직 작성하지 않았으므로 나중에 작성 후 적용하기
    // 추가로 이 변수는 기본 방 클래스로 넘기기 / 보상이 픽업이 아닌 경우도 있기 때문에
    //std::weak_ptr<class CPickUp> mReward;

protected:
    std::vector<std::weak_ptr<class CMeshComponent>> mBackgroundMesh;
    std::weak_ptr<CMeshComponent> mShadeMesh1;               //방 음영

    //방의 진입점(문의 위치)
    //진입점이라고 쓴 이유는 문이 없을때도 항상 이 위치로만 입장해야하기 떄문
    //그냥 방 크기(13, 7) / 2 가 진입점임
    //1300 : 700 || 텍스쳐 크기 => 650 : 350 || 위치는 +-325 : +-175

protected:
    virtual void OnEnterRoom() = 0;
    virtual void WinRoom() = 0;
    virtual void OnExitRoom() = 0;

    virtual bool WinCheck() = 0;

    void SetEnableAll(bool Enable = false);

    void AdjustRoomPos();
public:
    bool SetInitRoom(FVector2 Coord, const std::vector<std::pair<int, FVector2>>& Objs);//여기에 정보를 넣어서 방 초기화하기 / 지금은 방의 좌표 정보만 들어가지만 나중에는 방 내부의 정보들도 포함되어야 함
    virtual void Reset(bool HardReset = false) override; //방 초기화(클리어 상태는 초기화 x) / 하드 리셋시 클리어도 초기화 몬스터 전부 생성
    //방의 현재 상태를 격자로 내보내 주는 함수 필요
    //방의 크기가 다른 경우를 고려하여 가상함수로 만들어 둠
    const virtual FVector2 GetUnitCoordInGrid(FVector3 WorldPos);
    void ConnectRoom(std::weak_ptr<CRoombase> Room);

    virtual void GenerateRoom(FVector2 Direction, int Min, int Max, int& Current);

public:
    const ERoomType GetRoomType() const { return mRoomType; }
    const ERoomShape GetRoomShape() const { return mShape; }
};
