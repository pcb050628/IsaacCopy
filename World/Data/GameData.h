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

#include "../GameObjectStructure.h"

class CGameData : CAsset
{
public:
	virtual ~CGameData() = 0;
protected:
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer;

protected: //읽기
	bool Load(const TCHAR* FileName, rapidjson::Document& Doc);

	//쓰기
	void StartWrite(); 
	bool EndWrite(); 

public:
	virtual bool WriteData(const TCHAR* FileName) = 0;
	virtual bool Read(const TCHAR* FileName) = 0;
};
