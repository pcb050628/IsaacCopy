#pragma once

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

enum class EOpenRequirement
{
    Clear,   //클리어시 자동으로 열림
    Key,    //열쇠로 열림
    Key2,   //열쇠 두개로 열림
    Blast,  //폭파로 열림
    Blast2, //폭파 두번으로 열림
};

enum class EObjectType
{
    //방 외부 객체
    PlayerCharacter,
    Room,
    Door,
    Item,
    //방 내부 객체
    Tear,
    Monster,
    Obstacle,
    Pickup,
    End,
};

enum class EObstacleType
{
    None,
    Rock,   //지상유닛의 이동을 방해하지만 폭파로 파괴 가능한 장애물
    Fire,   //유닛의 이동을 방해하지만 공격으로 파괴 가능한 장애물
    Hole,   //지상유닛의 이동을 방해하지만 특수한 경우 이동 가능한 장애물 (땅이 메워지거나 하는 경우 <- 문 앞이 뚫려있거나 하면 메워줌)
    Block,  //지상유닛의 이동을 방해하지만 특수한 경우 파괴 가능한 장애물 (열쇠를 사용하거나 등 / 기본적으로는 폭파로도 파괴 불가능)
    Button, //유닛의 이동을 방해하지않고 상호작용이 가능한 오브젝트
};

enum class EItemType
{
    Active,
    Passive,
};

enum class EPickupType
{
    Heart,
    Key,
    Bomb,
    Coin,
    Card,
    Pill,
    None,
};