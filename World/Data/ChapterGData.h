#pragma once
#include "GameData.h"

class CChapterGData :
    public CGameData
{
public:
    CChapterGData();
    virtual ~CChapterGData();
private:
	FChapterData mData;

public:
	virtual bool Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer) override;
	virtual bool Read(const TCHAR* FileName) override;

public:
	void ContainData(FChapterData Data) { mData = Data; }
	const FChapterData GetData() const { return mData; }
};

