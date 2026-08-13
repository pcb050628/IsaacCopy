#include "GlobalCollision.h"

#include "World/CollisionInfoManager.h"

void CGlobalCollision::SetGlobalCollision()
{
	CCollisionInfoManager* InfoManager = CCollisionInfoManager::GetInst();

	if (InfoManager)
	{
		InfoManager->CreateChannel("OverlapAll");
		InfoManager->CreateProfile("OverlapAll", "OverlapAll", true);
		//=========================================================================================================
		InfoManager->CreateChannel("Player");
		InfoManager->CreateProfile("Player", "Player", true);

		//몬스터 허트박트
		InfoManager->CreateChannel("Monster");
		InfoManager->CreateProfile("Monster", "Monster", true);

		//몬스터 히트박스
		InfoManager->CreateChannel("ContactHit_Monster");
		InfoManager->CreateProfile("ContactHit_Monster", "ContactHit_Monster", true);
		InfoManager->SetProfileInteraction("Monster", "ContactHit_Monster", ECollisionInteraction::Ignore);

		//픽업
		InfoManager->CreateChannel("PickUp");
		InfoManager->CreateProfile("PickUp", "PickUp", true);
		InfoManager->SetProfileInteraction("Door", "PickUp", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Obstacle", "PickUp", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Monster", "PickUp", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "PickUp", ECollisionInteraction::Ignore);

		//장애물
		InfoManager->CreateChannel("Obstacle");
		InfoManager->CreateProfile("Obstacle", "Obstacle", true, ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("Player", "Obstacle", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Obstacle", "Player", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Monster", "Obstacle", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Obstacle", "Monster", ECollisionInteraction::Block);

		//장애물 히트박스
		InfoManager->CreateChannel("ContactHit_Obstacle");
		InfoManager->CreateProfile("ContactHit_Obstacle", "ContactHit_Obstacle", true);
		InfoManager->SetProfileInteraction("Obstacle", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Monster", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Obstacle", "ContactHit_Obstacle", ECollisionInteraction::Ignore);

		//폭파
		InfoManager->CreateChannel("Blasting");
		InfoManager->CreateProfile("Blasting", "Blasting", true);
		InfoManager->SetProfileInteraction("ContactHit", "Blasting", ECollisionInteraction::Ignore);

		//눈물
		InfoManager->CreateChannel("Tear");
		InfoManager->CreateProfile("Tear", "Tear", true);
		InfoManager->SetProfileInteraction("Pickup", "Tear", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Monster", "Tear", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Obstacle", "Tear", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("Blasting", "Tear", ECollisionInteraction::Ignore);

		//문
		InfoManager->CreateChannel("Door");
		InfoManager->CreateProfile("Door", "Door", true, ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Door", "Monster", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Monster", "Door", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Player", "Door", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Door", "Player", ECollisionInteraction::Overlap);

		InfoManager->SetProfileInteraction("Obstacle", "Door", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "Door", ECollisionInteraction::Ignore);

		//벽도 프로필 생성해야함
		//벽은 모두 블락에 탄막-> 오버랩 / 탄이 벽을 통과하는 경우가 있음
		//					문-> 무시 / 문은 항상 벽에 붙어있음
		InfoManager->CreateChannel("Wall");
		InfoManager->CreateProfile("Wall", "Wall", true, ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Tear", "Wall", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Door", "Wall", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("Wall", "Wall", ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("Player", "Wall", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Wall", "Player", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Monster", "Wall", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Wall", "Monster", ECollisionInteraction::Block);
	}
}
