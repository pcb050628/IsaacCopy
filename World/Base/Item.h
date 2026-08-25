#pragma once
#include "GameDefinition.h"
#include "../UnitData.h"

class CItem :
	public CGameDefinition
{
public:
	CItem(EItemType Type, bool hit = false, bool shot = false, bool dead = false, bool kill = false
		, bool enterRoom = false, bool winRoom = false, bool exitRoom = false, bool enterChapter = false
		, bool disposable = false);
	virtual ~CItem() = 0;

public:
	virtual bool Init();

protected:
	//아이템의 타입 - 패시브 | 액티브
	const EItemType mItemType;
	//아이템 획득 시 출력 될 텍스트
	std::string mQuote;
	//참인 경우 스탯의 배율 변경 - 플레이어 유닛은 기본적으로 스탯과 스탯 배율을 지님
	bool mbIsAttributeMagnification = false;
	//아이템의 추가 스탯
	FUnitAttribute mUnitAdditionalAttribute = FUnitAttribute();

	//스프라이트 렌더러 먼저 만들자 그냥 미치겠다.
	//장애물에서 아이템 렌더를 위한 아이템 애니메이션
	//머리, 몸을 애니메이션 이름으로 저장하고 보내주는 이유는
	//유닛이 움직이는 방향에 따라 다른 스프라이트 또는 애니메이션으로 바꿔줘야 하기 때문
	std::string mItemSpriteName;
	std::string mItemAnimDataPath; //<-나중에 필요하지 않다면 제거하기
	//참인 경우 이미지를 추가 렌더하는 것이 아니라 렌더 이미지를 변경한다.
	bool mbIsHeadAnimOverride = false;
	bool mbIsBodyAnimOverride = false;
	std::string mHeadAnimDataFileName;
	std::string mHeadAnimDataFilePath;
	std::string mHeadAnimName;

	std::string mBodyAnimDataFileName;
	std::string mBodyAnimDataFilePath;
	std::string mBodyAnimName;

	//1회용 아이템인지 - 액티브 | 1회용이라면 사용 후 아이템 제거
	const bool mbIsDisposable;
	//에너지 - 액티브
	int mEnergyMax = 0;
	int mCurrentEnergy = 0;

public: //발동 조건들
	//피격 
	const bool bIsOnHit;
	//공격
	const bool bIsOnShot;
	//사망 
	const bool bIsOnDead;
	//적 처치
	const bool bIsOnKill;
	//방 입장 
	const bool bIsEnterRoom;
	//방 클리어 
	const bool bIsOnWinRoom;
	//방 퇴장 
	const bool bIsExitRoom;
	//챕터 입장
	const bool bIsEnterChapter;

	virtual void OnHit();
	virtual void OnShot();
	virtual void OnKill();
	virtual void OnDead();
	virtual void OnEnterRoom();
	virtual void OnWinRoom();
	virtual void OnExitRoom();
	virtual void OnEnterChapter();
	virtual void OnKillEnemy();
	virtual void OnUpdatecShooter(const std::weak_ptr<class CCharacter>& character);

public:
	const bool GetIsMagnification() const { return mbIsAttributeMagnification; }
	FUnitAttribute GetAttribute() const { return mUnitAdditionalAttribute; }
	const bool GetIsHeadAnimOverride() const { return mbIsHeadAnimOverride; }
	const bool GetIsBodyAnimOverride() const { return mbIsBodyAnimOverride; }
	const std::string& GetHeadAnimName() { return mHeadAnimName; }
	const std::string& GetBodyAnimName() { return mBodyAnimName; }

	//아이템의 특수효과들에 대해서
	// 아래의 모든 효과들의 중요한 점은 
	// 모든 효과는 활성화되는 시기를 가져야 한다.
	// 활성화의 판단은 아이템에서 내부적으로 하고
	// 대표적으로 기준이 되는 것들은 방의 이동 또는 챕터의 이동, 플레이어의 피격 같은 것들이 있다.
	//가장 흔한 눈물 효과들
	//  속성부여 -> EUnitEffect + FTearAttribute
	//  눈물의 경로 변경 -> 함수 오버라이드 + 내부 함수
	//  눈물의 분열 -> 내부 함수 있음
	//      (그냥 충돌시 눈물 추가 생성하면됨 | 분열 개수에 따라 방향 계산하고 이어서 생성되는 눈물에는 분열 스택 1낮춰서 전달하면 분열이 여러번 되어도 문제없음)
	//
	//다음으로 흔한 캐릭터 효과들
	//  능력치 변경 -> 그냥 계산해서 넣으면 됨
	//  면역 -> EUnitEmmune 이미 만들어 둠
	//  특수효과 -> EUnitEffect 이미 만들어 둠
	//  피격시 효과 -> 간단하게 아이템의 함수를 OnHit 에 등록하거나 아이템 컨테이너에서 OnHit 발생시 반복문으로 모든 아이템의 OnHit 을 실행시켜주면 됨
	//  부활 -> 아직 안만들어 뒀지만 간단하게 만들 수 있음 | 마리오 마냥 ExtraLife 만들어서 넣어두면 됨
	//  
	//마지막으로 게임 전체 효과들
	//  챕터 단위 효과 -> 만들어 두진 않았지만 간단하게 챕터 시작시 호출해주면 되고 | The Stairway같은 아이템의 경우 StartRoom 에 만들어 주면 된다 - 호출 시점은 마찬가지로 챕터 시작 시
	//  방 단위 효과 -> 방 입장시 호출 - OnRoomEnter 만들어 둠
	//  게임 룰 변경 -> GameRuleManager 에 만들어두고 변경하는 식으로 하면 될듯
	//  그외 -> Death's list 같은 아이템이 있는데(방에서 아이템이 정해준 순서대로 적을 죽이면 능력치 증가를 주는 아이템) 이런 것들도 발동 시기같은 것들이 다 정해져 있기 때문에 그에 맞춰서 만들면 되겠다.
	//          Lost Soul 의 경우도(Familiar 가 피격당하지 않고 보스 클리어시 보상) 챕터 단위 생성
	//

	//액티브 아이템인 경우 아이템 사용을 위한 함수
	//패시브 아이템의 UseItem 을 호출할경우 false 를 반환함
	virtual bool UseItem();

	virtual void OnGetItem(const std::weak_ptr<class CCharacter>& character) = 0;
};

