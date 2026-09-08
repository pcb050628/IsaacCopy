#include "RunGData.h"
#include "ChapterGData.h"
#include "PlayerGData.h"

CRunGData::CRunGData()
	:CGameData(EGDataType::Run)
{
}

CRunGData::~CRunGData()
{
}

bool CRunGData::Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer)
{
	Writer.Key("Seed");
	Writer.Int(mData.Seed);
	Writer.Key("Level");
	Writer.Int(mData.CurrentChapterLevel);

	CChapterGData chapterGD;
	Writer.Key("Chapters");
	Writer.StartArray();
	for (FChapterData cd : mData.Chapters)
	{
		chapterGD.ContainData(cd);
		Writer.StartObject();
		assert(chapterGD.Write(Writer) && "데이터 저장 실패");
		Writer.EndObject();
	}
	Writer.EndArray();

	CPlayerGData playerGD;
	playerGD.ContainData(mData.Player);
	Writer.Key("Player");
	Writer.StartObject();
	playerGD.Write(Writer);
	Writer.EndObject();

    return true;
}

bool CRunGData::Read(const TCHAR* FileName)
{
	rapidjson::Document d;
	if (!Load(FileName, d))
		return false;

	mData.Seed = d["Seed"].GetInt();
	mData.CurrentChapterLevel = d["Level"].GetInt();

	CChapterGData chapterGD;
	const rapidjson::Value& chptrArray = d["Chapters"];
	int size = chptrArray.Size();
	mData.Chapters.reserve(size);
	for (const rapidjson::Value& item : d["Chapters"].GetArray())
	{
		chapterGD.Read(item);
		mData.Chapters.push_back(chapterGD.GetData());
	}

	CPlayerGData playerGD;
	playerGD.Read(d["Player"]);
	mData.Player = playerGD.GetData();

    return true;
}

bool CRunGData::Read(const rapidjson::Value& Val)
{
    return false;
}
