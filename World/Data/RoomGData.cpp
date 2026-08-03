#include "RoomGData.h"

CRoomGData::~CRoomGData()
{
}

bool CRoomGData::WriteData(const TCHAR* FileName)
{
    StartWrite();
    writer.Key("ID");
    writer.Int(mData.ID);

    writer.Key("CoordX");
    writer.Int(static_cast<int>(mData.Coord.x));
    writer.Key("CoordY");
    writer.Int(static_cast<int>(mData.Coord.y));

    writer.Key("Clear");
    writer.Bool(mData.Clear);

    writer.Key("OBJS");
    writer.StartArray();
    //여기에 오브젝트들 입력
    for (int i = 0; i < mData.CurrentObjs.size(); i++)
    {
        std::string Name = "OBJ_" + i;
        writer.Key(Name.c_str());
        writer.StartObject();
        writer.Key("ID");
        writer.Int(mData.CurrentObjs[i].ID);
        writer.Key("CoordX");
        writer.Int(static_cast<int>(mData.CurrentObjs[i].Coord.x));
        writer.Key("CoordY");
        writer.Int(static_cast<int>(mData.CurrentObjs[i].Coord.y));
        writer.EndObject();
    }

    writer.EndArray();
    if (!EndWrite())
        return false;

    const TCHAR* BasePath = CPathManager::FindPath("Data");

    TCHAR FullPath[MAX_PATH] = {};

    lstrcpy(FullPath, BasePath);
    lstrcat(FullPath, FileName);

    std::ofstream outFile(FullPath, std::ios::out);
    if (!outFile.is_open())
        return false;

    outFile << buffer.GetString();

    outFile.close();
    return true;
}

//원래 안전장치같은게 좀 있어야하긴 하는데 다 빼놨음
//rapidJson Value 자체에서 값의 타입을 검사하는 함수가 있지만
//일단은 사용하지 않았다.
bool CRoomGData::Read(const TCHAR* FileName)
{
    rapidjson::Document d;
    if (!Load(FileName, d))
        return false;

    d["ID"].GetInt();
    d["CoordX"].GetInt();
    d["CoordY"].GetInt();
    d["Clear"].GetBool();

    const rapidjson::Value& objArray = d["OBJ"];
    int size = objArray.Size();
    mData.CurrentObjs.reserve(size);
    for (int i = 0; i < size; i++)
    {
        const rapidjson::Value& obj = objArray[i];
        mData.CurrentObjs[i].ID = obj["ID"].GetInt();
        mData.CurrentObjs[i].Coord = FVector2(obj["CoordX"].GetInt(), obj["CoordY"].GetInt());
    }
    
    return true;
}
