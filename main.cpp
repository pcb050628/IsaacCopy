

#include "Engine.h"
#include "resource.h"

#include "RenderManager.h"
#include "Asset/AssetManager.h"

#include "World/MainWorld.h"
#include "World/Chapter.h"
#include "World/DebugChapter.h"
#include "World/WorldManager.h"

#include "World/GlobalCollision.h"
#include "World/Data/GameDataManager.h"

#ifdef _DEBUG

#pragma comment(lib,"GameEngine_Debug.lib")

#else

#pragma comment(lib,"GameEngine.lib")

#endif


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//UNREFERENCED_PARAMETER : 인자 미사용 경고를 제거하는데 사용되는 매크로
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!CEngine::GetInst()->Init(hInstance, TEXT("MyGame"), IDI_ICON1, IDI_ICON1, 1300, 700))
	{
		CEngine::GetInst()->DestroyInst();
		return 0;
	}

	CRenderManager::GetInst()->CreateLayer("Tear", 5, ERenderSortType::None);
	CRenderManager::GetInst()->CreateLayer("Head", 4, ERenderSortType::Y);
	CRenderManager::GetInst()->CreateLayer("Body", 3, ERenderSortType::Y);

	CGlobalCollision::SetGlobalCollision();
	if (!CAssetManager::GetInst()->AddSubManager<CGameDataManager>(EAssetType::GameData))
	{
		return 0;
	}

	//만들어야하는 레벨
	//-메인 레벨(게임 시작 전 메인 화면)
	//-로딩 레벨(챕터 진입 전 레벨)
	//-디버그 레벨(디버그 빌드는 항상 이쪽으로 진입)
	//	디버그 레벨에서는 방을 찍고 저장한다.
	CWorldManager::GetInst()->CreateWorld<CDebugChapter>(false);
	//CWorldManager::GetInst()->CreateWorld<CChapter>(false);

	int Ret = CEngine::GetInst()->Run();

	CEngine::GetInst()->DestroyInst();

	return Ret;
}