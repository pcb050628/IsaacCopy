#include "GameFinalizer.h"

#include "LogManager.h"

#include "GameRuleManager.h"

#include "World/WorldManager.h"
#include "../Chapter.h"

void CGameFinalizer::Finalier()
{
    std::shared_ptr<CChapter> chapter = std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock());
    if (!chapter || chapter->GetLevel() == -1)
        return;
    if (!CGameRuleManager::GetInst()->SaveData(chapter))
    {
        LOG_DEBUG("세이브 실패");
    }
}
