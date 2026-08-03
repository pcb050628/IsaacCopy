#pragma once
#include "GameData.h"

class CRoomGData :
    public CGameData
{
public:
	CRoomGData();
	virtual ~CRoomGData() override;

private:
	FRoomData mData;

public:
	virtual bool Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer) override;
	virtual bool Read(const TCHAR* FileName) override;
	virtual bool Read(const rapidjson::Value& Val);

public:
	void ContainData(FRoomData& Data) { mData = Data; }
	const FRoomData& GetData() const { return mData; }
};

