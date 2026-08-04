#include "AnimGData.h"

#include "Asset/AssetManager.h"
#include "Asset/AnimationManager.h"

CAnimGData::CAnimGData()
	:CGameData(EGDataType::Anim)
{
}

CAnimGData::~CAnimGData()
{
}

bool CAnimGData::Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer)
{
	mData.Name;
	mData.Frames;

	Writer.Key("Name");
	Writer.String(mData.Name.c_str());

	Writer.Key("Frames");
	Writer.StartArray();
	for (int i = 0; i < mData.Frames.size(); i++)
	{
		std::string Name = "Start" + std::to_string(i + 1);
		Writer.Key(Name.c_str());
		Writer.StartObject();
		Writer.Key("StartX");
		Writer.Double(mData.Frames[i].Start.x);
		Writer.Key("StartY");
		Writer.Double(mData.Frames[i].Start.y);
		Writer.Key("SizeX");
		Writer.Double(mData.Frames[i].Size.x);
		Writer.Key("SizeY");
		Writer.Double(mData.Frames[i].Size.y);
		Writer.EndObject();
	}
	Writer.EndArray();

	return false;
}

bool CAnimGData::Read(const TCHAR* FileName)
{
	rapidjson::Document d;
	if (!Load(FileName, d))
		return false;

	mData.Name = d["Name"].GetString();

	const rapidjson::Value& frameArray = d["Frames"];
	int size = frameArray.Size();
	mData.Frames.reserve(size);
	for (int i = 0; i < size; i++)
	{
		const rapidjson::Value& f = frameArray[i];
		mData.Frames[i].Start = FVector2(static_cast<float>(f["StartX"].GetDouble()), static_cast<float>(f["StartY"].GetDouble()));
		mData.Frames[i].Size = FVector2(static_cast<float>(f["SizeX"].GetDouble()), static_cast<float>(f["SizeY"].GetDouble()));
	}
	return true;
}

bool CAnimGData::Read(const rapidjson::Value& Val)
{
	mData.Name = Val["Name"].GetString();

	const rapidjson::Value& frameArray = Val["Frames"];
	int size = frameArray.Size();
	mData.Frames.reserve(size);
	for (int i = 0; i < size; i++)
	{
		const rapidjson::Value& f = frameArray[i];
		mData.Frames[i].Start = FVector2(static_cast<float>(f["StartX"].GetDouble()), static_cast<float>(f["StartY"].GetDouble()));
		mData.Frames[i].Size = FVector2(static_cast<float>(f["SizeX"].GetDouble()), static_cast<float>(f["SizeY"].GetDouble()));
	}
	return true;
}

bool CAnimGData::MakeAnim()
{
	std::shared_ptr<CAnimationManager> mgr = CAssetManager::GetInst()->GetSubManager<CAnimationManager>(EAssetType::Animation2D);
	if (!mgr || mData.Frames.empty())
		return false;

	if (!mgr->CreateAnimation(mData.Name))
		return false;

	for (int i = 0; i < mData.Frames.size(); ++i)
	{
		mgr->AddFrame(mData.Name, mData.Frames[i].Start, mData.Frames[i].Size);
	}
	return true;
}
