#include "ChapterGData.h"
#include "RoomGData.h"

CChapterGData::CChapterGData()
	:CGameData(EGDataType::Chapter)
{
}

CChapterGData::~CChapterGData()
{
}

bool CChapterGData::Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer)
{
	writer.Key("Level");
	writer.Int(mData.ChapterLevel);

	writer.Key("Rooms");
	writer.StartArray();
	for (int i = 0; i < mData.Rooms.size(); ++i)
	{
		static CRoomGData data;
		data.ContainData(mData.Rooms[i]);

		std::string Name = "Room_" + std::to_string(i + 1);
		writer.Key(Name.c_str());
		writer.StartObject();writer.Key("ID");
		if (!data.Write(Writer))
			return false;
		writer.EndObject();
	}
	return true;
}

bool CChapterGData::Read(const TCHAR* FileName)
{
    rapidjson::Document d;
    if (!Load(FileName, d))
        return false;

    mData.ChapterLevel = d["Level"].GetInt();

    const rapidjson::Value& roomArray = d["Rooms"];
    int size = roomArray.Size();
    mData.Rooms.reserve(size);
    for (int i = 0; i < size; ++i)
    {
        const rapidjson::Value& val = roomArray[i];
        static CRoomGData data;
		if (!data.Read(val))
			return false;
        mData.Rooms[i] = data.GetData();
    }

    return true;
}
