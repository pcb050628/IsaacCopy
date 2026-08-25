#include "Item.h"

CItem::CItem(EItemType Type, bool hit, bool shot, bool dead, bool kill, bool enterRoom, bool winRoom, bool exitRoom, bool enterChapter, bool disposable)
    :CGameDefinition(EObjectType::Item), mItemType(Type)
    , bIsOnHit(hit), bIsOnShot(shot), bIsOnDead(dead), bIsOnKill(kill), bIsEnterRoom(enterRoom), bIsOnWinRoom(winRoom), bIsExitRoom(exitRoom)
    , bIsEnterChapter(enterChapter), mbIsDisposable(disposable)
{
}

CItem::~CItem()
{
}

bool CItem::Init()
{
    //아이템은 플레이어에게 종속되기 떄문에 월드에 할당 되지 않음
    //외부에서 생성하는 함수도 액터로 생성하지 않고 
    return true;
}

void CItem::OnHit()
{
}

void CItem::OnShot()
{
}

void CItem::OnKill()
{
}

void CItem::OnDead()
{
}

void CItem::OnEnterRoom()
{
}

void CItem::OnWinRoom()
{
}

void CItem::OnExitRoom()
{
}

void CItem::OnEnterChapter()
{
}

void CItem::OnKillEnemy()
{
}

void CItem::OnUpdatecShooter(const std::weak_ptr<class CCharacter>& character)
{
}

bool CItem::UseItem()
{
    return false;
}

void CItem::OnGetItem(const std::weak_ptr<class CCharacter>& character)
{
}
