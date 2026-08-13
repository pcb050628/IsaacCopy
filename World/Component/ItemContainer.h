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
	std::vector<std::shared_ptr<class CItem>> mItems;
	std::unordered_map<std::string, std::vector<std::weak_ptr<class CItem>>> mItemMap;
	std::vector<std::shared_ptr<class CItem>> mActiveItems;
	int mActiveContainLimit = 1;

	//layer 아이템을 얻으면 외형적인게 추가되는데 이걸 layer 마냥 쌓아간다.
	//아무리 많아봐야 20개를 넘기도 힘들겠지만
	//좀 불안하긴함
	std::weak_ptr<class CMeshComponent> mHeadMesh;
	std::weak_ptr<class CMeshComponent> mBodyMesh;

	std::vector<std::weak_ptr<class CAnimation2DComponent>> mHeadAnim;
	std::vector<std::weak_ptr<class CAnimation2DComponent>> mBodyAnim;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Destroy();

public:
	void ContainItem(int ID);

public:
	void OnHit();
	void OnShot();
	void OnDead();
	void OnKill();
	void EnterRoom();
	void OnWinRoom();
	void ExitRoom();
	void EnterChapter();

public:
	void PlayHeadAnim(bool Play, int Frame = 0);
	void PlayBodyAnim(bool Play, int Frame = 0);
	void SetHeadDirection(FVector2 Dir);
	void SetBodyDirection(FVector2 Dir);

	void SetMeshComponent(const std::weak_ptr<class CMeshComponent>& Head, const std::weak_ptr<class CMeshComponent>& Body);

private:

};

