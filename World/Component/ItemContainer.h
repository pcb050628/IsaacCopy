#pragma once
#include "World\ActorComponent.h"

class CItemContainer :
	public CActorComponent
{
public:
	CItemContainer();
	virtual ~CItemContainer();

protected:
	std::weak_ptr<class CCharacter> mOwnerCharacter;
	std::weak_ptr<class CTearShooter> mCharacterTearShooter;
	//아이템을 들고 있어야하는데
	//아이템은 추가하는 경우는 많지만 지우는 일은 거의 없다
	//접근해야하는 일이 많다.
	//순회가 좋은 벡터가 낫겠지?
	//순회도 필요한 만큼만 한느게 낫겠찌?
	//맵으로 만들어서 용도에 따라 다르게 저장하고 필요할때마다 필요한 맵의 벡터만 순회한다.
	std::map<int, std::shared_ptr<class CItem>> mContainedItems;
	std::unordered_map<std::string, std::vector<std::weak_ptr<class CItem>>> mItemCallbackMap;
	std::vector<std::shared_ptr<class CItem>> mActiveItems;
	int mActiveContainLimit = 1;
	int mActiveFocused = 0;

	//layer 아이템을 얻으면 외형적인게 추가되는데 이걸 layer 마냥 쌓아간다.
	//아무리 많아봐야 20개를 넘기도 힘들겠지만
	//좀 불안하긴함
	std::vector<std::weak_ptr<class CMeshComponent>> mHeadMesh;
	std::vector<std::weak_ptr<class CMeshComponent>> mBodyMesh;

	std::vector<std::weak_ptr<class CAnimation2DComponent>> mHeadAnim;
	std::vector<std::weak_ptr<class CAnimation2DComponent>> mBodyAnim;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Destroy();

public:
	void ContainItem(int ID);
	bool IsContained(int ID);
	void ReleaseItem(int ID);

	std::string GetActiveItemName();

public:
	void UseItem();

	void OnGetHit(std::weak_ptr<class CGameObject> from, int& dmg);
	void OnShot();
	void OnDead();
	void OnKill();
	void EnterRoom();
	void OnWinRoom();
	void ExitRoom();
	void EnterChapter();

public:
	void PlayHeadAnim(bool Stop, int Frame = 0);
	void PlayBodyAnim(bool Stop, int Frame = 0);
	void SetHeadDirection(FVector2 Dir);
	void SetBodyDirection(FVector2 Dir);

	void SetHitEffect(bool Enable, float intensity, const FVector4& Color);

	void SetHeadAnimPlayTime(float Time);
	void SetBodyAnimPlayTime(float Time);

	void SetHeadRenderEnable(bool Enable);
	void SetBodyRenderEnable(bool Enable);

private:
	void ContainPassive(const std::shared_ptr<CItem>& item);
	void ContainActive(const std::shared_ptr<CItem>& item);

};

