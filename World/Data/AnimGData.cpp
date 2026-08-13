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
	Writer.Key("Name");
	Writer.String(mData.Name.c_str());

	Writer.Key("TextureName");
	Writer.String(mData.TextureName.c_str());

	Writer.Key("TexturePath");
	Writer.String(mData.TexturePath.c_str());

	Writer.Key("Frames");
	Writer.StartArray();
	for (int i = 0; i < mData.Frames.size(); i++)
	{
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

	return true;
}

bool CAnimGData::Read(const TCHAR* FileName)
{
	rapidjson::Document d;
	if (!Load(FileName, d))
		return false;

	mData.Name = d["Name"].GetString();
	mData.TextureName = d["TextureName"].GetString();
	mData.TexturePath = d["TexturePath"].GetString();

	const rapidjson::Value& frameArray = d["Frames"];
	int size = frameArray.Size();
	mData.Frames.reserve(size);
	for (const rapidjson::Value& item : d["Frames"].GetArray())
	{
		bool te = item.IsObject();
		FFrameData frame;
		frame.Start = FVector2(static_cast<float>(item["StartX"].GetDouble()), static_cast<float>(item["StartY"].GetDouble()));
		frame.Size = FVector2(static_cast<float>(item["SizeX"].GetDouble()), static_cast<float>(item["SizeY"].GetDouble()));
		mData.Frames.push_back(frame);
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
		FFrameData frame;
		frame.Start = FVector2(static_cast<float>(f["StartX"].GetDouble()), static_cast<float>(f["StartY"].GetDouble()));
		frame.Size = FVector2(static_cast<float>(f["SizeX"].GetDouble()), static_cast<float>(f["SizeY"].GetDouble()));
		mData.Frames.push_back(frame);
	}
	return true;
}

bool CAnimGData::MakeAnim()
{
	std::shared_ptr<CAnimationManager> mgr = CAssetManager::GetInst()->GetSubManager<CAnimationManager>(EAssetType::Animation2D);
	if (mData.Frames.empty() || !mgr)
		return false;

	if (!mgr->CreateAnimation(mData.Name))
		return false;

	std::wstring fileName(mData.TexturePath.begin(), mData.TexturePath.end());
	std::wstring path = CPathManager::FindPath("Texture");
	path += fileName;
	if (!mgr->SetTextureFullPath(mData.Name, mData.TextureName, path.c_str()))
		return false;
	
	mgr->SetAnimationTextureType(mData.Name, EAnimation2DTextureType::SpriteSheet);

	for (int i = 0; i < mData.Frames.size(); ++i)
	{
		mgr->AddFrame(mData.Name, mData.Frames[i].Start, mData.Frames[i].Size);
	}
	return true;
}
