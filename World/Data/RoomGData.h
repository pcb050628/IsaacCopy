#pragma once
#include "GameData.h"

class CRoomGData :
    public CGameData
{
public:
	virtual ~CRoomGData() override;

private:
	FRoomData mData;

public:
	virtual bool WriteData(const TCHAR* FileName) override;
	virtual bool Read(const TCHAR* FileName) override;

public:
	void ContainData(FRoomData Data) { mData = Data; }
	const FRoomData GetData() const { return mData; }
};

