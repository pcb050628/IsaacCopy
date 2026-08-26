#pragma once
#include "World\Actor.h"
#include "GameClass.h"

#define GAMEOBJCLASS(OBJ, ID) public: constexpr static int GObjID = ID; virtual const int GetGClassID() override;\
                                static std::weak_ptr<CGameObject> InstanceGObj(const FVector2& Coord, bool OnFocus, int Level);\
                                private: const static bool IsRegister;


//여기에 인스턴싱 내용을 넣기 / 를 위해서 챕터에 객채별 인스턴싱 템플릿 만들기 / 저장된 런의 재생성을 위해서 챕터를 선택해서 생성하는 방식으로 바꾸기 , 그냥 문자열 매개변수 하나 넣기
#define REGISTER_GAMEOBJCLASS(GAMEOBJ, NAME, TYPE)const int GAMEOBJ::GetGClassID() { return GAMEOBJ::GObjID; }\
                                     std::weak_ptr<CGameObject> GAMEOBJ::InstanceGObj(const FVector2& Coord, bool OnFocus, int Level) {\
                                    if(0 == Level)\
                                        return std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock())->MakeObject<GAMEOBJ>(NAME, TYPE, GObjID, Coord, OnFocus);   \
                                    else\
                                        return std::dynamic_pointer_cast<CChapter>(CWorldManager::GetInst()->GetWorld().lock())->MakeObject<GAMEOBJ>(NAME, TYPE, GObjID, Coord, OnFocus);\
                                    }\
                                    const bool GAMEOBJ::IsRegister = CGameClassContainer::GetInst()->RegisterGameClass(GAMEOBJ::GObjID, NAME, &GAMEOBJ::InstanceGObj) ? []() -> bool {LOG_DEBUG(GAMEOBJ::GObjID, " - ", NAME); return true;}() : false;


/// 아이디 구조
/// 1. 방
/// 2. 몬스터
/// 3. 플레이어 캐릭터
/// 6. 눈물
/// </summary>

 
class CGameObject :
    public CActor, public CGameClass
{
public:
    virtual const int GetGClassID() = 0;
public:
    CGameObject(EObjectType Type);
    CGameObject(const CGameObject& src);
    CGameObject(CGameObject&& src) noexcept;
    virtual ~CGameObject() = 0;
    
protected:
    bool mbIsTemporary = false;

    std::weak_ptr<class CChapter> mChapter;

public:
    virtual bool Init();
    virtual void Reset(bool HardReset = false) = 0; //상태를 초기화

    void SetTemporary(const bool Val) { mbIsTemporary = Val; }
    const bool GetIsTemporary() const { return mbIsTemporary; }

    std::weak_ptr<class CChapter> GetChapter() { return mChapter; }

    virtual void ReturnToChapter();
};

