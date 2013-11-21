#include "Animation.h"
#include "Affectors.h"

namespace Meepo
{

CAnimation::CAnimation(int duration)
    : m_iDurationMS(duration)
{}

void CAnimation::Apply(CAnimationInstance& inst)
{
    for (std::vector<CAffectorBase*>::iterator it = m_vecAffector.begin();
        it != m_vecAffector.end(); 
        ++it)
        (*it)->Apply(inst);
}

void CAnimation::SetReplayMode(ReplayMode mode)
{
    m_replayMode = mode;
}

Meepo::ReplayMode CAnimation::GetReplayMode() const
{
    return m_replayMode;
}

}
