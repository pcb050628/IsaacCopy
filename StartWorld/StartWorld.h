#pragma once
#include "World\World.h"

class CStartWorld :
    public CWorld
{
public:
    CStartWorld();
    virtual ~CStartWorld();

private:
    std::weak_ptr<class CScreenActor> mTitle;
    std::weak_ptr<class CScreenActor> mMenu;
    std::weak_ptr<class CScreenActor> mCharacterMenu;
    std::weak_ptr<class CStartCameraActor> mCam;
    int mFocused = 0;
    bool mbMoveToChapter = false;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostRender() override;

private:
    void OnUp();
    void OnDown();
    void OnRight();
    void OnLeft();
    void OnSubmit();
    void OnEscape();

    void MoveStart();
    void MoveEnd();
};

//
// 무엇을 해야할까요
// 첫화면 오브젝트 하나하고
// 메뉴 오브젝트 하나하고
// 다른건 나중에 만든다고 치고 캐릭터 선택 오브젝트 하나하고
//  캐릭터 선택창 만들기
//  벡터로 만들어두고 이미지 렌더하는데
//  정면과 그 반대편의 y값을 정해두고 이어가면서 보간하기
//  전체 배열을 절반으로 나누고 오프셋을 더한 위치를 정면과 반대면으로 지정 후 y와 크기를 보간해서 설정함
// 
//