#pragma once
#include "GameClass.h"

#define GAMEDEFCLASS(OBJ, ID) public: constexpr static int GObjID = ID; virtual const int GetGClassID() override;\
                                static std::shared_ptr<CGameDefinition> InstanceGDef(const std::weak_ptr<CGameClass>& owner);\
                                private: const static bool IsRegister;

#define REGISTER_GAMEDEFCLASS(GAMEDEF, NAME, TYPE)const int GAMEDEF::GetGClassID() { return GAMEDEF::GObjID; }\
                                        std::shared_ptr<CGameDefinition> GAMEDEF::InstanceGDef(const std::weak_ptr<CGameClass>& owner) {\
                                            std::shared_ptr<GAMEDEF> gdef =std::make_shared<GAMEDEF>(); \
                                            return gdef->Init(owner) ? std::dynamic_pointer_cast<CGameDefinition>(gdef) : std::shared_ptr<CGameDefinition>();   \
                                        }\
                           const bool GAMEDEF::IsRegister = CGameClassContainer::GetInst()->RegisterGameClass(GAMEDEF::GObjID, NAME, &GAMEDEF::InstanceGDef);

class CGameDefinition :
    public CGameClass, public std::enable_shared_from_this<CGameClass>
{
public:
    CGameDefinition(EObjectType Type);
    CGameDefinition(const CGameDefinition& src);
    CGameDefinition(CGameDefinition&& src) noexcept;
    virtual ~CGameDefinition() = 0;

protected:
    std::weak_ptr<CGameClass> mOwner;

    std::string mName;

public:
    virtual bool Init(const std::weak_ptr<CGameClass>& Owner) = 0;
    virtual void Reset(bool HardReset = false) = 0;

public:
    const std::string& GetName() { return mName; }

public:
    std::weak_ptr<CGameClass> GetGCPtr()
    {
        return shared_from_this();
    }
};

