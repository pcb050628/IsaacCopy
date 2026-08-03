#pragma once
#include "Chapter.h"

class CDebugChapter :
    public CChapter
{
public:
	CDebugChapter();
	virtual ~CDebugChapter();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

