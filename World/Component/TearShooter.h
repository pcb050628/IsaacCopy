#pragma once
#include "World\ActorComponent.h"

class CTearShooter :
    public CActorComponent
{
public:
    CTearShooter();
    virtual ~CTearShooter();

private:
    //딱히 필요한게 없음
    //그냥 외부에서 받아서 생성만 해주면 됨

    //그렇다면 이 컴포넌트를 거쳐야 하는 이유는 뭔가?
    //역할을 나눠줌

};

