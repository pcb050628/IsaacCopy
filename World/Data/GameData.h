#pragma once
#include "Math/Vector2.h" 
#include <fstream>
#include <vector>

#include "Json/RapidJson.h"
#include "Json/Document.h"
#include "Json/stringbuffer.h"
#include "Json/writer.h"

#include "Asset/Asset.h"
#include "Asset/PathManager.h"

#include "GameObjectStructure.h"

enum class EGDataType
{
	Run,
	Chapter,
	Room,
	Player,
	Item,
	End,
};

class CGameData : CAsset
{
public:
	CGameData(EGDataType Type);
	virtual ~CGameData() = 0;
private:
	const EGDataType mGDType;
protected:
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer;

protected: //읽기
	bool Load(const TCHAR* FileName, rapidjson::Document& Doc);

	//쓰기
	void StartWrite(); 
	bool EndWrite(); 

	virtual bool Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer) = 0;

public:
	virtual bool WriteData(const TCHAR* FileName);
	virtual bool Read(const TCHAR* FileName) = 0;

public:
	const EGDataType GetType() const { return mGDType; }
};
