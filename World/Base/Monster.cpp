#include "Monster.h"

#include "LogManager.h"

#include "Asset/AssetManager.h"
#include "Asset/SoundManager.h"

#include "World/SoundComponent.h"

#include "../Manager/GameRuleManager.h"

#include "Tear.h"

CMonster::CMonster()
	:CUnitbase(EObjectType::Monster)
{
}

CMonster::CMonster(const CMonster& src)
	:CUnitbase(src)
{
}

CMonster::CMonster(CMonster&& src) noexcept
	:CUnitbase(std::move(src))
{
}

CMonster::~CMonster()
{
}

void CMonster::Update(float DeltaTime)
{
	CUnitbase::Update(DeltaTime);
}

void CMonster::Reset(bool HardReset)
{
	mCurrentHP = mMaxHP;
}

void CMonster::SetEnable(bool Enable)
{
	CUnitbase::SetEnable(Enable);
	CTimeManager::ClearTimer(mMumblingTimer);
}

void CMonster::GetHit(std::weak_ptr<CGameObject> From)
{
	if (From.expired())
		return;
	if (mHurtSound.size() > 1)
	{
		int rand = CGameRuleManager::GetInst()->GenerateRandomI();
		mSoundPlayer.lock()->mSound = mHurtSound[rand % mHurtSound.size()].lock();
	}
	else if(!mHurtSound.empty())
	{
		mSoundPlayer.lock()->mSound = mHurtSound[0].lock();
	}
	mSoundPlayer.lock()->Play();

	std::shared_ptr<CGameObject> obj = From.lock();
	EObjectType t = obj->GetObjType();
	if (EObjectType::Tear == t)
	{
		std::shared_ptr<CTear> tear = std::dynamic_pointer_cast<CTear>(obj);
		t = tear->GetOwnerType();
		obj = tear->GetShooterOwner().lock();
	}

	if (EObjectType::PlayerCharacter == t || EObjectType::Monster == t)
	{
		std::shared_ptr<CUnitbase> unit = std::static_pointer_cast<CUnitbase>(obj);
		FUnitAttribute atrbt = unit->GetAttribute();
		mCurrentHP -= atrbt.Damage;
		LOG_DEBUG(GetName(), "유닛이 ", obj->GetActorTag(), " 에게 공격받았습니다\n 피해량: ", atrbt.Damage);
		if (mCurrentHP <= 0)
			ReturnToChapter(); //이 부분 나중에 수정하기 -> 애니메이션 같은거 출력하고 리턴하기 | 일단 비활성화는 해야함
	}
	else if (EObjectType::Obstacle == t)
	{

	}
}

void CMonster::SetMumblingSound(const std::string& soundName, float Time, bool loop)
{
	std::shared_ptr<CSoundManager> mgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
	std::weak_ptr<CSound> sound =  mgr->FindSound(soundName);
	if (sound.expired())
		return;

	mMumblingSound = sound;
	mMumblingTimer = CTimeManager::SetTimer(Time, loop, this, &CMonster::PlayerMumbling);
}

void CMonster::SetMumblingSound(float Time, bool loop)
{
	mMumblingTimer = CTimeManager::SetTimer(Time, loop, this, &CMonster::PlayerMumbling);
}

void CMonster::PlayerMumbling()
{
	mSoundPlayer.lock()->mSound = mMumblingSound.lock();
	mSoundPlayer.lock()->Play();
}
