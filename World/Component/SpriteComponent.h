#pragma once
#include "World\MeshComponent.h"

#include "../Data/GameObjectStructure.h"

class CSpriteComponent :
    public CMeshComponent
{
public:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& src);
	CSpriteComponent(CSpriteComponent&& src) noexcept;
	virtual ~CSpriteComponent();

protected:
	std::shared_ptr<class CCBufferSprite2D> mCBufferSprite;
	std::weak_ptr<class CTexture> mTexture;
	FSpriteData mSpriteData;
	
public:
	void SetTexture(const std::string& Name);

	void SetSpriteData(const std::string& dataName);
	void SetSpriteData(const std::weak_ptr<class CSpriteGData>& d);

	void SetUV(FVector2 Start, FVector2 Size);
	void SetUV(float StartX, float StartY, float SizeX, float SizeY);

	void SetSymmetry(bool Value);
	void SetFlip(bool Value);

public:
	virtual bool Init();
	virtual void Render();
	virtual void Destroy();

	void SetAnimComp(const std::weak_ptr<class CAnimation2DComponent>& AnimComp) = delete;
};

