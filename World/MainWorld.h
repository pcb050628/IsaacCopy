#pragma once

#include "World/World.h"

//
// 새로운 월드를 만들 때
// 1. 객체에 따라서 각자 다른 함수로 월드에서 생성하며 생성된 객체들은 월드에서 두가지 상태로 관리된다.
//		1. 할당됨, 2. 할당되지 않음
//		방에서 객체 생성을 요청하면 무조건 생성하는 것이 아니라 필요한 객체가 할당되지 않은 상태로 남아있는지를 확인하고 반환한다.
// 2. 벽은 가로 4개 세로 4개 총 8개만 만든다.
//		벽이 필요한 상호작용이 거의 없어서 모든 방에서 이 8개의 벽을 함께 사용한다.
// 3. 생성 단계는 방->방 초기화(객체 생성)->모든 방 연결 종료->문 생성(일단은 방마다 다른 객체로 생성한다.)->현재 방 설정->
//					플레이어 생성
// 4. 
//
class CMainWorld :
    public CWorld
{
public:
	CMainWorld();
	virtual ~CMainWorld();

protected:
	std::vector<std::weak_ptr<class CCameraComponent>> mCameraList;
	int mCameraIndex = 0;

public:
	virtual bool Init();


public:
	void ChangeCamera();
};
