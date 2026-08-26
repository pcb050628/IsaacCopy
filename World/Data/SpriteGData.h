#pragma once
#include "GameData.h"

class CSpriteGData :
    public CGameData
{
public:
	CSpriteGData();
	virtual ~CSpriteGData() override;

private:
	FSpriteData mData;

public:
	virtual bool Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer) override;
	virtual bool Read(const TCHAR* FileName) override;
	virtual bool Read(rapidjson::Value& Val);

public:
	void ContainData(FSpriteData& Data) { mData = Data; }
	const FSpriteData& GetData() const { return mData; }
};

