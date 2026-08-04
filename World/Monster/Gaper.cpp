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

	//tlqkf 뭐 하나 하려면 해야하는게 시발 10개는 쳐 넘어가는거같아
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
