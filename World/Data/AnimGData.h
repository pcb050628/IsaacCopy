#pragma once
#include "GameData.h"

class CAnimGData :
    public CGameData
{
public:
	CAnimGData();
	virtual ~CAnimGData() override;

private:
	FAnimationData mData;

public:
	virtual bool Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer) override;
	virtual bool Read(const TCHAR* FileName) override;
	virtual bool Read(const rapidjson::Value& Val);

public:
	void ContainData(FAnimationData& Data) { mData = Data; }
	const FAnimationData& GetData() const { return mData; }

	bool MakeAnim();
};

