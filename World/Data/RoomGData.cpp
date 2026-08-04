#include "RoomGData.h"

#include "../Data/GameObjectStructure.h"

CRoomGData::CRoomGData()
    :CGameData(EGDataType::Room)
{
}

CRoomGData::~CRoomGData()
{
}

bool CRoomGData::Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer)
{
    Writer.Key("ID");
    Writer.Int(mData.ID);

    Writer.Key("CoordX");
    Writer.Int(static_cast<int>(mData.Coord.x));
    Writer.Key("CoordY");
    Writer.Int(static_cast<int>(mData.Coord.y));

    Writer.Key("Clear");
    Writer.Bool(mData.Clear);

    Writer.Key("Doors");
    Writer.StartArray();
    for (int i = 0; i < mData.Doors.size(); ++i)
    {
        Writer.Bool(mData.Doors[i]);
    }
    Writer.EndArray();

    Writer.Key("OBJS");
    Writer.StartArray();
    //여기에 오브젝트들 입력
    for (int i = 0; i < mData.CurrentObjs.size(); i++)
    {
        std::string Name = "OBJ_" + std::to_string(i + 1);
        Writer.Key(Name.c_str());
        Writer.StartObject();
        Writer.Key("ID");
        Writer.Int(mData.CurrentObjs[i].ID);
        Writer.Key("CoordX");
        Writer.Int(static_cast<int>(mData.CurrentObjs[i].Coord.x));
        Writer.Key("CoordY");
        Writer.Int(static_cast<int>(mData.CurrentObjs[i].Coord.y));
        Writer.EndObject();
    }

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

    mData.ID = d["ID"].GetInt();
    mData.Coord = FVector2(static_cast<float>(d["CoordX"].GetInt()), static_cast<float>(d["CoordY"].GetInt()));
    mData.Clear = d["Clear"].GetBool();
    
    const rapidjson::Value& doorArray = d["Doors"];
    int size = doorArray.Size();
    mData.Doors.reserve(size);
    for (int i = 0; i < size; ++i)
    {
        const rapidjson::Value& val = doorArray[i];
        mData.Doors.push_back(val.GetBool());
    }

    const rapidjson::Value& objArray = d["OBJ"];
    size = objArray.Size();
    mData.CurrentObjs.reserve(size);
    for (int i = 0; i < size; i++)
    {
        const rapidjson::Value& obj = objArray[i];
        FRoomObjectData objData;
        objData.ID = obj["ID"].GetInt();
        objData.Coord = FVector2(static_cast<float>(obj["CoordX"].GetInt()), static_cast<float>(obj["CoordY"].GetInt()));
        mData.CurrentObjs.push_back(objData);
    }
    
    return true;
}

bool CRoomGData::Read(const rapidjson::Value& Val)
{
    mData.ID = Val["ID"].GetInt();
    mData.Coord = FVector2(static_cast<float>(Val["CoordX"].GetInt()), static_cast<float>(Val["CoordY"].GetInt()));
    mData.Clear = Val["Clear"].GetBool();

    const rapidjson::Value& doorArray = Val["Doors"];
    int size = doorArray.Size();
    mData.Doors.reserve(size);
    for (int i = 0; i < size; ++i)
    {
        const rapidjson::Value& door = doorArray[i];
        mData.Doors.push_back(door.GetBool());
    }

    const rapidjson::Value& objArray = Val["OBJ"];
    size = objArray.Size();
    mData.CurrentObjs.reserve(size);
    for (int i = 0; i < size; i++)
    {
        const rapidjson::Value& obj = objArray[i];
        FRoomObjectData objData;
        objData.ID = obj["ID"].GetInt();
        objData.Coord = FVector2(static_cast<float>(obj["CoordX"].GetInt()), static_cast<float>(obj["CoordY"].GetInt()));
        mData.CurrentObjs.push_back(objData);
    }
    return true;
}
