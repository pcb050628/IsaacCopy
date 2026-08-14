#pragma once
#include "World\Actor.h"
#include "../../GameObjectEnums.h"

#define GAMEOBJCLASS(OBJ, ID) public: constexpr static int GObjID = ID; virtual const int GetGObjID() override;\
                                static std::weak_ptr<CGameObject> InstanceGObj(const FVector2& Coord, bool OnFocus, int Level);\
                                private: const static bool IsRegister;

#define GAMEDEFCLASS(OBJ, ID) public: constexpr static int GObjID = ID; virtual const int GetGObjID() override;\
                                static std::shared_ptr<CGameObject> InstanceGObj();\
                                private: const static bool IsRegister;

//여기에 인스턴싱 내용을 넣기 / 를 위해서 챕터에 객채별 인스턴싱 템플릿 만들기 / 저장된 런의 재생성을 위해서 챕터를 선택해서 생성하는 방식으로 바꾸기 , 그냥 문자열 매개변수 하나 넣기
#define REGISTER_GAMEOBJCLASS(GAMEOBJ, NAME, TYPE)const int GAMEOBJ::GetGObjID() {\
                                          return GAMEOBJ::GObjID;                                  \
                                        }\
                                     std::weak_ptr<CGameObject> GAMEOBJ::InstanceGObj(const FVector2& Coord, bool OnFocus, int Level) {\
                                    if(0 == Level)\
                                        return std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock())->MakeObject<GAMEOBJ>(NAME, TYPE, GObjID, Coord, OnFocus);   \
                                    else\
                                        return std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock())->MakeObject<GAMEOBJ>(NAME, TYPE, GObjID, Coord, OnFocus);\
                                    }\
                           const bool GAMEOBJ::IsRegister = CGameClassContainer::GetInst()->RegisterGameClass<GAMEOBJ>(GAMEOBJ::GObjID, &GAMEOBJ::InstanceGObj);

#define REGISTER_GAMEDEFCLASS(GAMEDEF, NAME, TYPE)const int GAMEDEF::GetGObjID() { return GAMEDEF::GObjID; }\
                                        std::shared_ptr<CGameObject> GAMEDEF::InstanceGObj() {\
                                            std::shared_ptr<GAMEDEF> gdef =std::make_shared<GAMEDEF>(); \
                                            return gdef->Init() ? std::dynamic_pointer_cast<CGameObject>(gdef) : std::shared_ptr<CGameObject>();   \
                                        }\
                           const bool GAMEDEF::IsRegister = CGameClassContainer::GetInst()->RegisterGameClass<GAMEDEF>(GAMEDEF::GObjID, &GAMEDEF::InstanceGObj);

/// 아이디 구조
/// 1. 방
/// 2. 몬스터
/// 3. 플레이어 캐릭터
/// 6. 눈물
/// </summary>

 
class CGameObject :
    public CActor
{
public:
    virtual const int GetGObjID() = 0;
public:
    CGameObject(EObjectType Type);
    CGameObject(const CGameObject& src);
    CGameObject(CGameObject&& src) noexcept;
    virtual ~CGameObject() = 0;
    
protected:
    const EObjectType mObjType;
    const int mID;

    bool mbIsTemporary = false;

public:
    const int GetID() const { return mID; }
    const EObjectType GetObjType() const { return mObjType; }

    virtual void Reset(bool HardReset = false) = 0; //상태를 초기화

    void SetTemporary(const bool Val) { mbIsTemporary = Val; }
    const bool GetIsTemporary() const { return mbIsTemporary; }

    void ReturnToChapter();
private:
    static int GlobalID;
};

