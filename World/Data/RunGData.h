#pragma once
#include "GameData.h"
class CRunGData :
    public CGameData
{
public:
	CRunGData();
	virtual ~CRunGData() override;

private:
	FRunData mData;

public:
	virtual bool Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer) override;
	virtual bool Read(const TCHAR* FileName) override;
	virtual bool Read(const rapidjson::Value& Val);

public:
	void ContainData(FRunData& Data) { mData = Data; }
	const FRunData& GetData() const { return mData; }
};

