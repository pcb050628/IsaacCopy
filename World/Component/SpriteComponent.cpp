#include "SpriteComponent.h"

#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"
#include "Asset/CBufferSprite2D.h"
#include "Asset/Texture.h"

CSpriteComponent::CSpriteComponent()
{
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& src)
    :CMeshComponent(src)
{
}

CSpriteComponent::CSpriteComponent(CSpriteComponent&& src) noexcept
    :CMeshComponent(std::move(src))
{
}

CSpriteComponent::~CSpriteComponent()
{
}

void CSpriteComponent::SetTexture(const std::string& Name)
{
    auto TextureMgr = CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture);

    if (TextureMgr)
    {
        mTexture = TextureMgr->FindTexture(Name);
        AddTexture(0, mTexture);
    }
}

void CSpriteComponent::SetUV(FVector2 Start, FVector2 Size)
{
    if (mTexture.expired())
        return;

    const FTextureInfo* info = mTexture.lock()->GetTexture();
    mCBufferSprite->SetLTUV(Start.x / info->Width, Start.y / info->Height);
    mCBufferSprite->SetRBUV((Start.x + Size.x) / info->Width, (Start.y + Size.y) / info->Height);
}

void CSpriteComponent::SetUV(float StartX, float StartY, float SizeX, float SizeY)
{
    if (mTexture.expired())
        return;
    const FTextureInfo* info = mTexture.lock()->GetTexture();
    mCBufferSprite->SetLTUV(StartX / info->Width, StartY / info->Height);
    mCBufferSprite->SetRBUV((StartX + SizeX) / info->Width, (StartY + SizeY) / info->Height);
}

void CSpriteComponent::SetSymmetry(bool Value)
{
    mCBufferSprite->SetTextureSymmetry(Value);
}

void CSpriteComponent::SetFlip(bool Value)
{
    mCBufferSprite->SetTextureFlip(Value);
}

bool CSpriteComponent::Init()
{
    if (!CMeshComponent::Init())
        return false;

    mCBufferSprite = std::make_shared<CCBufferSprite2D>();
    if (!mCBufferSprite->Init())
        return false;

    return true;
}

void CSpriteComponent::Render()
{
    mCBufferSprite->UpdateBuffer();
    CMeshComponent::Render();
}

void CSpriteComponent::Destroy()
{
    CMeshComponent::Destroy();
}
