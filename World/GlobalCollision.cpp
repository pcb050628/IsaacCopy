#include "GlobalCollision.h"

#include "World/CollisionInfoManager.h"

void CGlobalCollision::SetGlobalCollision()
{
	CCollisionInfoManager* InfoManager = CCollisionInfoManager::GetInst();

	if (InfoManager)
	{
		InfoManager->CreateChannel("BlockAll");
		InfoManager->CreateProfile("BlockAll", "BlockAll", true, ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Static", "BlockAll", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Player", "BlockAll", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Monster", "BlockAll", ECollisionInteraction::Block);

		InfoManager->CreateChannel("OverlapAll");
		InfoManager->CreateProfile("OverlapAll", "OverlapAll", true);
		//=========================================================================================================
		InfoManager->CreateChannel("Player");
		InfoManager->CreateProfile("Player", "Player", true);

		InfoManager->CreateChannel("Monster");
		InfoManager->CreateProfile("Monster", "Monster", true);

		InfoManager->CreateChannel("ContactHit_Monster");
		InfoManager->CreateProfile("ContactHit_Monster", "ContactHit_Monster", true);
		InfoManager->SetProfileInteraction("Monster", "ContactHit_Monster", ECollisionInteraction::Ignore);

		InfoManager->CreateChannel("PickUp");
		InfoManager->CreateProfile("PickUp", "PickUp", true);
		InfoManager->SetProfileInteraction("Door", "PickUp", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Obstacle", "PickUp", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Monster", "PickUp", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "PickUp", ECollisionInteraction::Ignore);

		InfoManager->CreateChannel("Obstacle");
		InfoManager->CreateProfile("Obstacle", "Obstacle", true, ECollisionInteraction::Block);

		InfoManager->CreateChannel("ContactHit_Obstacle");
		InfoManager->CreateProfile("ContactHit_Obstacle", "ContactHit_Obstacle", true);
		InfoManager->SetProfileInteraction("Obstacle", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Monster", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Obstacle", "ContactHit_Obstacle", ECollisionInteraction::Ignore);

		InfoManager->CreateChannel("Blasting");
		InfoManager->CreateProfile("Blasting", "Blasting", true);
		InfoManager->SetProfileInteraction("ContactHit", "Blasting", ECollisionInteraction::Ignore);

		InfoManager->CreateChannel("Door");
		InfoManager->CreateProfile("Door", "Door", true, ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Player", "Door", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Obstacle", "Door", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "Door", ECollisionInteraction::Ignore);

		//벽도 프로필 생성해야함
		//벽은 모두 블락에 탄막-> 오버랩 / 탄이 벽을 통과하는 경우가 있음
		//					문-> 무시 / 문은 항상 벽에 붙어있음
	}
}
