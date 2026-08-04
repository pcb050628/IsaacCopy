#include "Gaper.h"


CGaper::CGaper()
{
}

CGaper::CGaper(const CGaper& src)
{
}

CGaper::CGaper(CGaper&& src) noexcept
{
}

CGaper::~CGaper()
{
}

bool CGaper::Init()
{
	if (!CWalker::Init())
		return false;

	if (!SetAnim("Gaper_Idle_Head", TEXT("Anim/Gaper_Idle_Head.txt"), true))
		return false;
	if (!SetAnim("Gaper_Idle_Body", TEXT("Anim/Gaper_Idle_Body.txt")))
		return false;

	//디버그 챕터에서 애니메이션 만들고 데이터 저장하기
	return true;
}

void CGaper::Update(float DeltaTime)
{
}

void CGaper::Destory()
{
}

void CGaper::Dead()
{
}

void CGaper::MoveToTarget()
{
}
