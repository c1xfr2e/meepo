#include "AnimationInstance.h"
#include "Animation.h"

namespace Meepo
{

CAnimationInstance::CAnimationInstance()
    : m_pAnimation(0), m_pTargetObject(0), m_bIsRunning(false), 
    m_iCurrentPosition(0), m_uStartTime(0)
{

}

CAnimationInstance::CAnimationInstance(CAnimation* animation, CElement* target)
    : m_pAnimation(animation), m_pTargetObject(target), m_bIsRunning(false),
    m_iCurrentPosition(0)
{

}

void CAnimationInstance::Start()
{
    m_bIsRunning = true;
    m_iCurrentPosition = 0;
    m_uStartTime = ::GetTickCount();
}

void CAnimationInstance::Stop()
{
    m_bIsRunning = false;
    m_iCurrentPosition = 0;
}

bool CAnimationInstance::Step()
{
    if (!m_bIsRunning)
        return  false;

    int eclipse = GetTickCount() - m_uStartTime;

    m_iCurrentPosition = eclipse;
    if (m_iCurrentPosition >= m_pAnimation->GetDuration())
    {
        m_pAnimation->Apply(*this);
        if (m_pAnimation->GetReplayMode() == ReplayMode_Once)
        {
            m_bIsRunning = false;
            m_iCurrentPosition = 0;
            m_uStartTime = 0;
        }
        if (m_pAnimation->GetReplayMode() == ReplayMode_Loop)
        {
            m_uStartTime = GetTickCount();
            m_iCurrentPosition = 0;
        }
        return true;
    }

    m_pAnimation->Apply(*this);
    return true;
}

}