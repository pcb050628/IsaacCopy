#include "DefaultRoom.h"
#include "../Base/Unitbase.h"

#include "../Manager/GameClassContainer.h"

#include "Asset/PathManager.h"
#include "World/WorldManager.h"
#include "../Chapter.h"

#include "World/MeshComponent.h"

REGISTER_GAMECLASS(CDefaultRoom, "Room_Default", EObjectType::Room);

CDefaultRoom::CDefaultRoom()
	:CRoombase(ERoomType::Normal, ERoomShape::Normal)
{
	
}

CDefaultRoom::~CDefaultRoom()
{
}

bool CDefaultRoom::Init()
{
	if (!CRoombase::Init())
		return false;

	//메시에 텍스쳐 넣기

	mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundLT"));
	//mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundRT"));
	//mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundLB"));
	//mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundRT"));

	mBackgroundMesh[0].lock()->SetRelativeScale(FVector2(1300.f, 700.f));
	mBackgroundMesh[0].lock()->SetMesh("TexRect");
	mBackgroundMesh[0].lock()->SetShader("Material");
	std::wstring path = CPathManager::FindPath("Texture");
	path += TEXT("Room\\Basement.png");
	mBackgroundMesh[0].lock()->AddTextureFullPath(0, "Basement", path.c_str());
	//uv 는 여기서 골라서 올려주기

	return true;
}

void CDefaultRoom::Update(float DeltaTime)
{
	CRoombase::Update(DeltaTime);
}

void CDefaultRoom::Destroy()
{
	CActor::Destroy();
}

void CDefaultRoom::OnEnterRoom()
{
}

void CDefaultRoom::WinRoom()
{
	mbIsRoomWin = true;
	//보상 스폰하기
}

void CDefaultRoom::OnExitRoom()
{
}

bool CDefaultRoom::WinCheck()
{
	bool flag = false;
	for (std::weak_ptr<CUnitbase> unit : mUnits)
	{
		if (unit.lock()->IsEnable())
		{
			return false;
		}
	}
	return true;
}