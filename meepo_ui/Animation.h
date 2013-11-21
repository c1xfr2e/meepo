#pragma once

#include "PreHeader.h"
#include <vector>

namespace Meepo
{

enum ReplayMode
{
    ReplayMode_Once,
    ReplayMode_Loop,
    ReplayMode_Bounce
};

class CAnimation
{
protected:
    std::vector<CAffectorBase*> m_vecAffector;
    int m_iDurationMS;
    ReplayMode m_replayMode;

public:
    CAnimation(int duration);

    void AddAffector(CAffectorBase* affector) { m_vecAffector.push_back(affector); }
    int GetDuration() const { return m_iDurationMS; }
    void Apply(CAnimationInstance& inst);
    void SetReplayMode(ReplayMode mode);
    ReplayMode GetReplayMode() const;

    static const int ms_iUpdateIntervalMS = 16;
};

}