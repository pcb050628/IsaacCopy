#pragma once
#include "EngineInfo.h"

class CImGuiSpriteMaker
{
private:
	std::shared_ptr<class CSpriteGData> gd;
	std::shared_ptr<struct FSpriteData> d;

	std::string Name;
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

	void DrawUpdate();
	void DrawFrame();
	void DrawImage();

	void MakeButton();

	void LoadTexture();

	void SaveButton();
	void LoadButton();
};

