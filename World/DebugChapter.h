#pragma once
#include "Chapter.h"

#include "../Debug/ImGuiAnimationMaker.h"
#include "../Debug/ImGuiRoomMaker.h"
#include "../Debug/ImGuiSpriteMaker.h"

class CDebugChapter :
    public CChapter
{
public:
	CDebugChapter();
	virtual ~CDebugChapter();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	CImGuiAnimationMaker animMaker;
	CImGuiRoomMaker roomMaker;
	CImGuiSpriteMaker spriteMaker;
	bool debugMode = true;
	EGDataType dataType = EGDataType::Anim;

	void CheckPlayerPos();

	void DebugDrawTogle();
};

