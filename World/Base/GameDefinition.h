#pragma once
#include "GameClass.h"

#define GAMEDEFCLASS(OBJ, ID) public: constexpr static int GObjID = ID; virtual const int GetGObjID() override;\
                                static std::shared_ptr<CGameObject> InstanceGObj();\
                                private: const static bool IsRegister;

#define REGISTER_GAMEDEFCLASS(GAMEDEF, NAME, TYPE)const int GAMEDEF::GetGObjID() { return GAMEDEF::GObjID; }\
                                        std::shared_ptr<CGameObject> GAMEDEF::InstanceGObj() {\
                                            std::shared_ptr<GAMEDEF> gdef =std::make_shared<GAMEDEF>(); \
                                            return gdef->Init() ? std::dynamic_pointer_cast<CGameObject>(gdef) : std::shared_ptr<CGameObject>();   \
                                        }\
                           const bool GAMEDEF::IsRegister = CGameClassContainer::GetInst()->RegisterGameClass<GAMEDEF>(GAMEDEF::GObjID, NAME, &GAMEDEF::InstanceGObj);

class CGameDefinition :
    public CGameClass
{
public:
    CGameDefinition();
    CGameDefinition(const CGameDefinition& src);
    CGameDefinition(CGameDefinition&& src);
    virtual ~CGameDefinition() = 0;

protected:


public:

private:


};

