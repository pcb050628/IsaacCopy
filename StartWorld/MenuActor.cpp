#include "MenuActor.h"

#include "Device.h"
#include "World/WorldManager.h"

#include "Asset/AssetManager.h"
#include "../World/Data/GameDataManager.h"

#include "../World/Chapter.h"

#include "../World/Component/SpriteComponent.h"

#include "../World/Data/RunGData.h"

CMenuActor::CMenuActor()
{
}

CMenuActor::~CMenuActor()
{
}

bool CMenuActor::Init()
{
	mBackground = CreateComponent<CSpriteComponent>("Root");
	mElementRun = CreateComponent<CSpriteComponent>("Ele_Run");
	mElementContinue = CreateComponent<CSpriteComponent>("Ele_Continue");
	mElementPointer = CreateComponent<CSpriteComponent>("Ele_Pointer");
	if (mBackground.expired() || mElementRun.expired() || mElementContinue.expired() || mElementPointer.expired())
		return false;

	std::shared_ptr<CSpriteComponent> bg = mBackground.lock();
	std::shared_ptr<CSpriteComponent> run = mElementRun.lock();
	std::shared_ptr<CSpriteComponent> con = mElementContinue.lock();
	std::shared_ptr<CSpriteComponent> pointer = mElementPointer.lock();

	bg->SetMesh("TexRect"); bg->SetShader("Sprite2D"); bg->SetRenderLayer(0);
	run->SetMesh("TexRect"); run->SetShader("Sprite2D"); run->SetRenderLayer(1);
	con->SetMesh("TexRect"); con->SetShader("Sprite2D"); con->SetRenderLayer(1);
	pointer->SetMesh("TexRect"); pointer->SetShader("Sprite2D"); pointer->SetRenderLayer(1);

	FResolution resol = CDevice::GetInst()->GetResolution();
	bg->SetWorldScale(resol.Width, resol.Height);
	run->SetRelativeScale(0.3f, 0.2f);
	con->SetRelativeScale(0.3f, 0.2f);
	pointer->SetRelativeScale(0.05f, 0.05f);

	run->SetRelativePos(0, 100.f);
	con->SetRelativePos(0, 0);
	pointer->SetRelativePos(-220.f, 70.f);

	bg->SetSpriteData("UI_Game_Menu_Background");
	run->SetSpriteData("UI_Game_Menu_Element_Run");
	con->SetSpriteData("UI_Game_Menu_Element_Continue");
	pointer->SetSpriteData("UI_Game_Menu_Pointer");

	return true;
}

void CMenuActor::Update(float DeltaTime)
{
	CActor::Update(DeltaTime);
}

void CMenuActor::UpdatePointerPos()
{
	if(0 == mFocused)
		mElementPointer.lock()->SetRelativePos(-220.f, 70.f);
	else
		mElementPointer.lock()->SetRelativePos(-220.f, -30.f);
}

int CMenuActor::OnUp()
{
	mFocused = abs(mFocused - 1);
	UpdatePointerPos();
	return 0;
}

int CMenuActor::OnDown()
{
	mFocused = (mFocused + 1) % 2;
	if (mFocused < 0)
		mFocused = 0;
	UpdatePointerPos();
	return 0;
}

int CMenuActor::OnRight()
{
	return 0;
}

int CMenuActor::OnLeft()
{
	return 0;
}

int CMenuActor::OnSubmit()
{
	switch (mFocused)
	{
	case 0: //캐릭터  창
		return 1;
	case 1: { //이어하기
		std::shared_ptr<CRunGData> check = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData)->PeekRandom<CRunGData>(EGDataType::Run).lock();
		if (check)
		{
			CChapter::SetLoadMode(true);
			return 0;
		}
		else
			return 2;
	}break;
	}
}

int CMenuActor::OnEscape()
{
	return -1;
}

//
// 초기화 단계에서 챕터에 접근하는 것 자체가 문제
// 하지만 접근하지 않고 만들 수 있는지?
// 액터의 보관 위치를 옮긴다면?
// 너무 큰 작업이긴 하지만 가능은 하다
// 액터들의 구분은?
// 룸/룸 멤버/눈물
// 딱히 풀링 하고싶지도 않은데
// 
//