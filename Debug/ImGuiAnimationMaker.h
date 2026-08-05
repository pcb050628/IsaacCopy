#pragma once
#include "EngineInfo.h"

class CImGuiAnimationMaker
{
private:
	std::shared_ptr<class CAnimGData> gd;
	std::shared_ptr<struct FAnimationData> d;

	std::string Name;
	std::string AnimPath;
	std::string TextureName;
	std::string TexturePath;

	FVector2 Start;
	FVector2 Size;

	int SelectedIdx = 0;

	std::weak_ptr<class CTexture> DrawingTex;

	std::string ErrorPopupID = "error";

	bool isOpen = true;

public:
	void Init();
	void Update();

private:
	void InputInfo();
	void InputFrame();

	void DrawUpdate();
	void DrawFrame();
	void DrawImage();

	void FrameList();
	void FrameClearButton();
	void AddFrameButton();
	void MakeButton();

	void LoadTexture();

	void SaveAnimButton();
	void LoadAnimButton();
};

