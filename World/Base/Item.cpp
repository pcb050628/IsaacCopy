#include "Item.h"

CItem::CItem(const std::string& Name, EItemType Type, bool hit, bool shot, bool dead, bool kill, bool enterRoom, bool winRoom, bool exitRoom, bool enterChapter, bool disposable)
    :CGameDefinition(EObjectType::Item), mItemType(Type)
    , bIsOnHit(hit), bIsOnShot(shot), bIsOnDead(dead), bIsOnKill(kill), bIsEnterRoom(enterRoom), bIsOnWinRoom(winRoom), bIsExitRoom(exitRoom)
    , bIsEnterChapter(enterChapter), mbIsDisposable(disposable)
{
    mName = Name;
}

CItem::~CItem()
{
}

bool CItem::Init(const std::weak_ptr<CGameClass>& Owner)
{
    //아이템은 플레이어에게 종속되기 떄문에 월드에 할당 되지 않음
    //외부에서 생성하는 함수도 액터로 생성하지 않고 
    return CGameDefinition::Init(Owner);
}

void CItem::OnGetHit()
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
