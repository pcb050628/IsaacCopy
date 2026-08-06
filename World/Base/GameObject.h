#pragma once
#include "World\Actor.h"

#define GAMECLASS(OBJ, ID) public: constexpr static int GObjID = ID;\
                                static std::weak_ptr<CGameObject> InstanceGObj(const FVector2& Coord, int Level);\
                                private: const static bool IsRegister;

//여기에 인스턴싱 내용을 넣기 / 를 위해서 챕터에 객채별 인스턴싱 템플릿 만들기 / 저장된 런의 재생성을 위해서 챕터를 선택해서 생성하는 방식으로 바꾸기 , 그냥 문자열 매개변수 하나 넣기
#define REGISTER_GAMECLASS(GAMEOBJ, NAME, TYPE) std::weak_ptr<CGameObject> GAMEOBJ::InstanceGObj(const FVector2& Coord, int Level) {\
                                    if(0 == Level)\
                                        return std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock())->MakeObject<GAMEOBJ>(NAME, TYPE, Coord);   \
                                    else\
                                        return std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock())->MakeObject<GAMEOBJ>(NAME, TYPE, Coord);\
                                    }\
                           const bool GAMEOBJ::IsRegister = CGameClassContainer::GetInst()->RegisterGameClass<GAMEOBJ>(GAMEOBJ::GObjID, &GAMEOBJ::InstanceGObj);

enum class EObjectType
{
    //방 외부 객체
    PlayerCharacter,
    Room,
    Door,
    //방 내부 객체
    Tear,
    Monster,
    Obstacle,
    Pickup,
    End,
};
 
class CGameObject :
    public CActor
{
public:
    CGameObject(EObjectType Type);
    CGameObject(const CGameObject& src);
    CGameObject(CGameObject&& src) noexcept;
    virtual ~CGameObject() = 0;
    
protected:
    const EObjectType mObjType;
    const int mID;

public:
    const int GetID() const { return mID; }
    const EObjectType GetObjType() const { return mObjType; }

    virtual void Reset(bool HardReset = false) = 0; //상태를 초기화

private:
    static int GlobalID;
};

