#pragma once
#include "Unitbase.h"

class CMonster :
    public CUnitbase
{
public:
    CMonster();
    CMonster(const CMonster& src);
    CMonster(CMonster&& src) noexcept;
    virtual ~CMonster() = 0;

protected:
    float mMaxHP = 10.f;
    float mCurrentHP = mMaxHP;

    std::unordered_map<std::string, std::string> mAnimMap; //애니메이션 이름, 애니메이션 파일이름

public:
    virtual void Reset(bool HardReset = false);

protected:
    //공용으로 사용할만한 기능들 생각하기
    //1. 플레이어 캐릭터 찾기 <- 이거는 월드가 제공할 수 있음
    //2. 플레이어 캐릭터 에게 가는 길 찾기 <- 이건 월드에서 받아서 멤버 변수로 저장
    void FindRouteToPlayerCharacter();
    //3. 플레이어 캐릭터 에게 갈 수 있는지 확인하기 <- 월드에서 받기
    bool CanGetToPlayerCharacter();

};

