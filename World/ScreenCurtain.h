#pragma once
#include "World\Actor.h"

class CScreenCurtain :
    public CActor
{
public:
    CScreenCurtain();
    virtual ~CScreenCurtain();

private:
    std::weak_ptr<class CSpriteComponent> mBackground;
    std::weak_ptr<class CSpriteComponent> mGround;
    std::weak_ptr<class CSpriteComponent> mCharacter;
    std::weak_ptr<class CSpriteComponent> mLight;
    float mDuration = 3.f;
    std::function<void()> mOnFadeOut;
    bool mbStartFade = false;

    float mTimeStack = 0.f;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    void Start();

    template<typename T>
    void SetOnFadeOutFunction(T* Obj, void(T::* Func)())
    {
        mOnFadeOut = std::bind(Func, Obj);
    }

private:
    void StartFade();
    void EndFade();
};

