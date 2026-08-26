#include "SpriteGData.h"

CSpriteGData::CSpriteGData()
    :CGameData(EGDataType::Sprite)
{
}

CSpriteGData::~CSpriteGData()
{
}

bool CSpriteGData::Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer)
{
	Writer.Key("Name");
	Writer.String(mData.Name.c_str());

	Writer.Key("TextureName");
	Writer.String(mData.TextureName.c_str());

	Writer.Key("TexturePath");
	Writer.String(mData.TexturePath.c_str());

	Writer.Key("Frame");
	Writer.StartObject();
	Writer.Key("StartX");
	Writer.Double(mData.UV.Start.x);
	Writer.Key("StartY");
	Writer.Double(mData.UV.Start.y);
	Writer.Key("SizeX");
	Writer.Double(mData.UV.Size.x);
	Writer.Key("SizeY");
	Writer.Double(mData.UV.Size.y);
	Writer.EndObject();

	return true;
}

bool CSpriteGData::Read(const TCHAR* FileName)
{
	rapidjson::Document d;
	if (!Load(FileName, d))
		return false;

	mData.Name = d["Name"].GetString();
	mData.TextureName = d["TextureName"].GetString();
	mData.TexturePath = d["TexturePath"].GetString();

	rapidjson::Value& f = d["Frame"];
	assert(f.IsObject());

	mData.UV.Start = FVector2(static_cast<float>(f["StartX"].GetDouble()), static_cast<float>(f["StartY"].GetDouble()));
	mData.UV.Size = FVector2(static_cast<float>(f["SizeX"].GetDouble()), static_cast<float>(f["SizeY"].GetDouble()));
	return true;
}

bool CSpriteGData::Read(rapidjson::Value& Val)
{
	mData.Name = Val["Name"].GetString();
	mData.TextureName = Val["TextureName"].GetString();
	mData.TexturePath = Val["TexturePath"].GetString();

	rapidjson::Value& f = Val["Frame"];
	assert(f.IsObject());

	mData.UV.Start = FVector2(static_cast<float>(f["StartX"].GetDouble()), static_cast<float>(f["StartY"].GetDouble()));
	mData.UV.Size = FVector2(static_cast<float>(f["SizeX"].GetDouble()), static_cast<float>(f["SizeY"].GetDouble()));
	return true;
}
