#pragma once
#include "Chapter.h"

#include "../Debug/ImGuiAnimationMaker.h"

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
	bool debugMode = true;

	void CheckPlayerPos();
};

