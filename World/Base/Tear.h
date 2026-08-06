#pragma once
#include "GameObject.h"
#include "../UnitData.h"

class CTear :
    public CGameObject
{
public:
	CTear();
	virtual ~CTear();
	//눈물(객체(게임 오브젝트))
	//무엇이 필요한가요
	//1. 리지드 바디 컴포넌트
	//2. 애니메이션 컴포넌트
	//3. 콜라이더
	// 
	//1. 생성을 요청한 객체
	//2. 여러 효과들 내장해야함(독이나 화상같은)
	//2. 여러 기능들 내장해야함(충돌시 분열하는 등)

protected:
	std::weak_ptr<class CRigidBodyComponent> mRigidBody;
	std::weak_ptr<class CColliderSphere2D> mHitBox;

	std::weak_ptr<class CMeshComponent> mMesh;
	std::weak_ptr<class CAnimation2DComponent> mAnimator;

	//생성 요청 객체 <- 그런데 생각해보니 이게 필요한 경우는 플레이어 뿐인데
	//플레이어라면 그냥 Chapter 에서 가져오면 되는거고 그럼 이걸 이렇게 둘 필요가 있나?
	//이거 말고 FUnitAttribute 로 받자 차라리
	//std::weak_ptr<class CUnitbase> mUnitOwner;
	FUnitAttribute mUnitOwnerAttribute;

public:
	virtual void Reset(bool HardReset = false);

public:

};

