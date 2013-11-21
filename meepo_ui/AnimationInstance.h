#pragma once

#include "PreHeader.h"

namespace Meepo
{

class CAnimationInstance
{
public:
    CAnimationInstance();
    CAnimationInstance(CAnimation* animation, CElement* target);
    void Start();
    void Stop();
    bool Step();
    bool IsRunning() const { return m_bIsRunning; }
    CElement* GetTargetObject() const { return m_pTargetObject; }
    int GetCurrentPosition() const { return m_iCurrentPosition; }
    void SetAnimation(CAnimation* animation) { m_pAnimation = animation; }
    void SetTargetObject(CElement* element) { m_pTargetObject = element; }

private:
    CAnimation* m_pAnimation;
    CElement* m_pTargetObject;
    int m_iCurrentPosition;
    bool m_bIsRunning;
    unsigned int m_uStartTime;
};

}
