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

		InfoManager->CreateChannel("ContactHit_Monster");
		InfoManager->CreateProfile("ContactHit_Monster", "ContactHit_Monster", true);

		InfoManager->CreateChannel("PickUp");
		InfoManager->CreateProfile("PickUp", "PickUp", true);

		InfoManager->CreateChannel("Obstacle");
		InfoManager->CreateProfile("Obstacle", "Obstacle", true, ECollisionInteraction::Ignore);

		InfoManager->CreateChannel("ContactHit_Obstacle");
		InfoManager->CreateProfile("ContactHit_Obstacle", "ContactHit_Obstacle", true);

		InfoManager->CreateChannel("Blasting");
		InfoManager->CreateProfile("Blasting", "Blasting", true);

		InfoManager->CreateChannel("Tear");
		InfoManager->CreateProfile("Tear", "Tear", true);

		InfoManager->CreateChannel("Door");
		InfoManager->CreateProfile("Door", "Door", true, ECollisionInteraction::Block);

		InfoManager->CreateChannel("Wall");
		InfoManager->CreateProfile("Wall", "Wall", true, ECollisionInteraction::Block);

		//몬스터 히트박스
		InfoManager->SetProfileInteraction("Monster", "ContactHit_Monster", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Monster", "Monster", ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("Monster", "Monster", ECollisionInteraction::Block);

		//픽업
		InfoManager->SetProfileInteraction("Door", "PickUp", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("PickUp", "Door", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Monster", "PickUp", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("PickUp", "Monster", ECollisionInteraction::Overlap);

		InfoManager->SetProfileInteraction("ContactHit", "PickUp", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("PickUp", "ContactHit", ECollisionInteraction::Ignore);

		//장애물

		InfoManager->SetProfileInteraction("Player", "Obstacle", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Obstacle", "Player", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Monster", "Obstacle", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Obstacle", "Monster", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Obstacle", "PickUp", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("PickUp", "Obstacle", ECollisionInteraction::Block);

		//장애물 히트박스
		InfoManager->SetProfileInteraction("Obstacle", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Monster", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Obstacle", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("PickUp", "ContactHit_Obstacle", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Obstacle", "PickUp", ECollisionInteraction::Ignore);

		//폭파
		InfoManager->SetProfileInteraction("ContactHit", "Blasting", ECollisionInteraction::Ignore);

		//눈물
		InfoManager->SetProfileInteraction("PickUp", "Tear", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("Tear", "PickUp", ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("Tear", "ContactHit_Monster", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit_Monster", "Tear", ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("ContactHit_Obstacle", "Tear", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("Tear", "ContactHit_Obstacle", ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("Tear", "Blasting", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("Blasting", "Tear", ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("Obstacle", "Tear", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Tear", "Obstacle", ECollisionInteraction::Overlap);

		InfoManager->SetProfileInteraction("Wall", "Tear", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Tear", "Wall", ECollisionInteraction::Overlap);

		InfoManager->SetProfileInteraction("Door", "Tear", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Tear", "Door", ECollisionInteraction::Overlap);

		//문

		InfoManager->SetProfileInteraction("Door", "Monster", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Monster", "Door", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Player", "Door", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Door", "Player", ECollisionInteraction::Overlap);

		InfoManager->SetProfileInteraction("Obstacle", "Door", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("ContactHit", "Door", ECollisionInteraction::Ignore);

		//벽도 프로필 생성해야함
		//벽은 모두 블락에 탄막-> 오버랩 / 탄이 벽을 통과하는 경우가 있음
		//					문-> 무시 / 문은 항상 벽에 붙어있음
		InfoManager->SetProfileInteraction("Tear", "Wall", ECollisionInteraction::Overlap);
		InfoManager->SetProfileInteraction("Door", "Wall", ECollisionInteraction::Ignore);
		InfoManager->SetProfileInteraction("Wall", "Wall", ECollisionInteraction::Ignore);

		InfoManager->SetProfileInteraction("Player", "Wall", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Wall", "Player", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Monster", "Wall", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("Wall", "Monster", ECollisionInteraction::Block);

		InfoManager->SetProfileInteraction("Wall", "PickUp", ECollisionInteraction::Block);
		InfoManager->SetProfileInteraction("PickUp", "Wall", ECollisionInteraction::Block);
	}
}
