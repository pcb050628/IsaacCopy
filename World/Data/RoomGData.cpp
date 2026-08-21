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

    Writer.Key("OBJS");
    Writer.StartArray();
    //여기에 오브젝트들 입력
    for (int i = 0; i < mData.InitObjs.size(); i++)
    {
        Writer.StartObject();
        Writer.Key("ID");
        Writer.Int(mData.InitObjs[i].ID);
        Writer.Key("CoordX");
        Writer.Int(static_cast<int>(mData.InitObjs[i].Coord.x));
        Writer.Key("CoordY");
        Writer.Int(static_cast<int>(mData.InitObjs[i].Coord.y));
        Writer.EndObject();
    }
    Writer.EndArray();

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
    
    const rapidjson::Value& objArray = d["OBJS"];
    int size = objArray.Size();
    mData.InitObjs.reserve(size);
    for (const rapidjson::Value& item : d["OBJS"].GetArray())
    {
        bool te = item.IsObject();
        FRoomObjectData objData;
        objData.ID = item["ID"].GetInt();
        objData.Coord = FVector2(static_cast<float>(item["CoordX"].GetInt()), static_cast<float>(item["CoordY"].GetInt()));
        mData.InitObjs.push_back(objData);
    }
    
    return true;
}

bool CRoomGData::Read(const rapidjson::Value& Val)
{
    mData.ID = Val["ID"].GetInt();
    mData.Coord = FVector2(static_cast<float>(Val["CoordX"].GetInt()), static_cast<float>(Val["CoordY"].GetInt()));
    mData.Clear = Val["Clear"].GetBool();

    const rapidjson::Value& objArray = Val["OBJ"];
    int size = objArray.Size();
    mData.InitObjs.reserve(size);
    for (int i = 0; i < size; i++)
    {
        const rapidjson::Value& obj = objArray[i];
        FRoomObjectData objData;
        objData.ID = obj["ID"].GetInt();
        objData.Coord = FVector2(static_cast<float>(obj["CoordX"].GetInt()), static_cast<float>(obj["CoordY"].GetInt()));
        mData.InitObjs.push_back(objData);
    }
    return true;
}
