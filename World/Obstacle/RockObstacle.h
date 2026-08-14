#pragma once
#include "../Base/Obstaclebase.h"

class CRockObstacle :
    public CObstaclebase
{
    GAMEOBJCLASS(CRockObstacle, 41);
public:
    CRockObstacle();
    virtual ~CRockObstacle();

protected:

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();
    virtual void Reset(bool Hard = false);

private:

public:

};

