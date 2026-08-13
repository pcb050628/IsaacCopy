#pragma once
#include "GameObject.h"
#include "World\WorldManager.h"
#include "../../GameObjectEnums.h"

//13 x 7 : 14 x 9
//117 : 98
#define ROOM_GRID_SIZE 25
#define ROOM_GRID_HALF ROOM_GRID_SIZE / 2


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
    //내부 셀의 개수
    FVector2 mRoomCellMax = FVector2(13, 7);
    //방 이미지의 실제 픽셀 크기
    FVector2 mRoomImageSize; 
    //방 내부의 월드 크기
    FVector2 mRoomSize;
    //방 내부 셀의 크기
    FVector2 mRoomCellSize;


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
    //방은 내부 객체에게 능동적으로 뭔가 하는게 없다.
    //리셋을 해주긴 해야하는데
    //리셋도 그냥 다 반납하고 다 받아오고 하면ㄷ ㅚ는거 아닌가?
    //비용 차이는 얼마나 있지?
    //  현재 O(n)
    //  변경 O(n) <- 챕터의 저장 방식을 해시+리스트로 해서 접근 및 할당이 굉장히 빠름
    //그럼 바꾼다고 치고
    //장애물은 필요한가?
    //플레이어에게 갈 수 있나를 알기위해서는
    //방의 장애물들의 위치를 알아야하는데
    //구멍이 난 곳도 있을 수 있고 벽으로 막힌 경우도 있을 수 있고
    //음 그건 그리드 맵으로 충분하지 않나? 
    //변경사항은 그게 변할때인데
    //그건 뭐 사라지거나 할때 일테니까
    //등록을 해제하는 과정이 필요할테고 
    //따라서 그 과정에서 되겠네
    std::unordered_map<int, EObstacleType> mObstacleGridMap;
    std::list<std::pair<int, bool>> mMonsters;
    //벽이 필요한데, 벽의 경우 나올 수 있는 모양들이 몇가지 없어고 비활성화될 일도 없어서 월드에 올려두고 돌려쓰는 형식으로 쓸 것
    //따라서 벽을 세팅하는 함수를 작성하거나 Reset 함수 내부에 작성해야한다
    
    FVector2 mRedFlag = FVector2(-1, -1); //넘을 수 없는 위치 / 사용하는 방은 L모양 하나뿐이지만 일단 만들어둠
    std::vector<std::pair<int, FVector2>> mMonsterInit;

    bool mbIsRoomWin = false;
    //보상 / 해당 클래스는 아직 작성하지 않았으므로 나중에 작성 후 적용하기
    // 추가로 이 변수는 기본 방 클래스로 넘기기 / 보상이 픽업이 아닌 경우도 있기 때문에
    //std::weak_ptr<class CPickUp> mReward;

protected:
    std::weak_ptr<class CMeshComponent> mShadeMesh1;               //방 음영
    std::vector<std::weak_ptr<class CSpriteComponent>> mBackgroundSprite;

    //방의 진입점(문의 위치)
    //진입점이라고 쓴 이유는 문이 없을때도 항상 이 위치로만 입장해야하기 떄문
    //그냥 방 크기(13, 7) / 2 가 진입점임
    //1300 : 700 || 텍스쳐 크기 => 650 : 350 || 위치는 +-325 : +-175

    virtual void OnEnterRoom() = 0;
    virtual void WinRoom() = 0; //여는데 조건이 있는 문을 제외하고 다 열기
    virtual void OnExitRoom() = 0;

    void CalculateSize();
public:

    virtual bool WinCheck() = 0;

    void AdjustRoomPos();
public:
    void SetCoord(FVector2 Coord) { mCoord = Coord; }
    FVector2 GetCoord() { return mCoord; }
    bool SetInitRoom(const std::vector<std::pair<int, FVector2>>& Objs);//여기에 정보를 넣어서 방 초기화하기 / 지금은 방의 좌표 정보만 들어가지만 나중에는 방 내부의 정보들도 포함되어야 함
    virtual void Reset(bool HardReset = false) override; //방 초기화(클리어 상태는 초기화 x) / 하드 리셋시 클리어도 초기화 몬스터 전부 생성

    void RegisterGObj(const std::weak_ptr<class CGameObject>& GObj, const FVector2& Coord);
    void DisregisterGObj(const std::weak_ptr<class CGameObject>& GObj, const FVector2& Coord);
  
    const FVector3 CoordToWorldPos(FVector2 Coord);
    const virtual FVector2 GetUnitCoordInGrid(FVector3 WorldPos);
    const FVector2 GetPlayerCoordInGrid();
    const bool CanGetToPlayerCharacter(FVector3 FromWorldPos);
    bool CheckNearCell(FVector2 Coord);
    bool CheckCell(FVector2 Coord);


    void ConnectRoom(std::weak_ptr<CRoombase> Room);
    bool HasNearRoom(FVector2 Dir);

    virtual void GenerateRoom(FVector2 Direction, int Min, int Max, int& Current);

public:
    const ERoomType GetRoomType() const { return mRoomType; }
    const ERoomShape GetRoomShape() const { return mShape; }

private:
};
