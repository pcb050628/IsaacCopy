#include "PlayerGData.h"

CPlayerGData::CPlayerGData()
    :CGameData(EGDataType::Player)
{
}

CPlayerGData::~CPlayerGData()
{
}

bool CPlayerGData::Write(rapidjson::Writer<rapidjson::StringBuffer>& Writer)
{
    Writer.Key("ID");
    Writer.Int(mData.ID);

    //픽업 데이터 
    //체력, 돈, 열쇠, 폭탄
    Writer.Key("Hearts");
    Writer.StartArray();
    //여기에 오브젝트들 입력
    for (int i = 0; i < mData.Hearts.size(); i++)
    {
        Writer.StartObject();
        Writer.Key("Type");
        Writer.Int(mData.Hearts[i].Type);
        Writer.Key("State");
        Writer.Int(mData.Hearts[i].State);
        Writer.EndObject();
    }
    Writer.EndArray();

    Writer.Key("Coin");
    Writer.Int(static_cast<int>(mData.Coins));
    Writer.Key("Key");
    Writer.Int(static_cast<int>(mData.Keys));
    Writer.Key("Bomb");
    Writer.Int(static_cast<int>(mData.Bombs));

    Writer.Key("ChapterX");
    Writer.Int(static_cast<int>(mData.ChapterCoord.x));
    Writer.Key("ChapterY");
    Writer.Int(static_cast<int>(mData.ChapterCoord.y));

    Writer.Key("RoomX");
    Writer.Int(static_cast<int>(mData.RoomCoord.x));
    Writer.Key("RoomY");
    Writer.Int(static_cast<int>(mData.RoomCoord.y));

    Writer.Key("Items");
    Writer.StartArray();
    //여기에 오브젝트들 입력
    for (int i = 0; i < mData.Items.size(); i++)
    {
        Writer.StartObject();
        Writer.Key("ID");
        Writer.Int(mData.Items[i].ID);
        Writer.Key("Charge");
        Writer.Int(mData.Items[i].ChargedEnerge);
        Writer.Key("Stack");
        Writer.Int(mData.Items[i].Stack);
        Writer.EndObject();
    }
    Writer.EndArray();

	return true;
}

bool CPlayerGData::Read(const TCHAR* FileName)
{
    rapidjson::Document d;
    if (!Load(FileName, d))
        return false;

    mData.ID = d["ID"].GetInt();

    const rapidjson::Value& heartArray = d["Hearts"];
    int size = heartArray.Size();
    mData.Hearts.reserve(size);
    for (const rapidjson::Value& item : d["Hearts"].GetArray())
    {
        FHeartData heartData;
        heartData.Type = item["Type"].GetInt();
        heartData.State = item["State"].GetInt();
        mData.Hearts.push_back(heartData);
    }

    mData.Coins = d["Coin"].GetInt();
    mData.Keys = d["Key"].GetInt();
    mData.Bombs = d["Bomb"].GetInt();

    mData.ChapterCoord = FVector2(static_cast<float>(d["ChapterX"].GetInt()), static_cast<float>(d["ChapterY"].GetInt()));
    mData.RoomCoord = FVector2(static_cast<float>(d["RoomX"].GetInt()), static_cast<float>(d["RoomY"].GetInt()));

    const rapidjson::Value& itemArray = d["Items"];
    size = itemArray.Size();
    for (const rapidjson::Value& item : d["Items"].GetArray())
    {
        bool te = item.IsObject();
        FItemData itemData;
        itemData.ID = item["ID"].GetInt();
        itemData.ChargedEnerge = item["Charge"].GetInt();
        itemData.Stack = item["Stack"].GetInt();
        mData.Items.push_back(itemData);
    }

	return true;
}

bool CPlayerGData::Read(const rapidjson::Value& Val)
{
    mData.ID = Val["ID"].GetInt();

    const rapidjson::Value& heartArray = Val["Hearts"];
    int size = heartArray.Size();
    mData.Hearts.reserve(size);
    for (const rapidjson::Value& item : Val["Hearts"].GetArray())
    {
        FHeartData heartData;
        heartData.Type = item["Type"].GetInt();
        heartData.State = item["State"].GetInt();
        mData.Hearts.push_back(heartData);
    }

    mData.Coins = Val["Coin"].GetInt();
    mData.Keys = Val["Key"].GetInt();
    mData.Bombs = Val["Bomb"].GetInt();

    mData.ChapterCoord = FVector2(static_cast<float>(Val["ChapterX"].GetInt()), static_cast<float>(Val["ChapterY"].GetInt()));
    mData.RoomCoord = FVector2(static_cast<float>(Val["RoomX"].GetInt()), static_cast<float>(Val["RoomY"].GetInt()));

    const rapidjson::Value& itemArray = Val["Items"];
    size = itemArray.Size();
    for (const rapidjson::Value& item : Val["Items"].GetArray())
    {
        bool te = item.IsObject();
        FItemData itemData;
        itemData.ID = item["ID"].GetInt();
        itemData.ChargedEnerge = item["Charge"].GetInt();
        itemData.Stack = item["Stack"].GetInt();
        mData.Items.push_back(itemData);
    }

    return true;
}
