#pragma once
#include "GameData.h"

class CPlayerGData :
    public CGameData
{
public:
	CPlayerGData();
	virtual ~CPlayerGData() override;

private:
	FPlayerData mData;

public:
	virtual bool Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer) override;
	virtual bool Read(const rapidjson::Value& Val);
	virtual bool Read(const TCHAR* FileName) override;

public:
	void ContainData(FPlayerData& Data) { mData = Data; }
	const FPlayerData& GetData() const { return mData; }
};

