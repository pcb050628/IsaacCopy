#include "Item.h"

CItem::CItem(EItemType Type, bool hit, bool shot, bool dead, bool kill, bool enterRoom, bool winRoom, bool exitRoom, bool enterChapter, bool disposable)
    :CGameObject(EObjectType::Item), mItemType(Type)
    , bIsOnHit(hit), bIsOnShot(shot), bIsOnDead(dead), bIsOnKill(kill), bIsEnterRoom(enterRoom), bIsOnWinRoom(winRoom), bIsExitRoom(exitRoom)
    , bIsEnterChapter(enterChapter), mbIsDisposable(disposable)
{
}

CItem::~CItem()
{
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

bool CItem::UseItem()
{
    return false;
}

void CItem::OnGetItem(const std::weak_ptr<class CCharacter>& character)
{
}
