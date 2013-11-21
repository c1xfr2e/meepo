#pragma once

#include "Element.h"
#include "KeyFrame.h"
#include "Interpolator.h"
#include <vector>
#include "AnimationInstance.h"
#include "StaticImage.h"

namespace Meepo
{

class CAffectorBase
{
public:
    virtual void Apply(CAnimationInstance& inst) = 0;
};

template <class T>
class CAffector : public CAffectorBase
{
public:
    typedef CKeyFrame<T> KeyFrame;

    virtual void Apply(CAnimationInstance& inst)
    {
        if (m_vecKeyFrames.empty())
            return;

        int curPosition = inst.GetCurrentPosition();
        CElement* object = inst.GetTargetObject();

        // take the fisrt keyframe who's position is less than current position 
        // as the left keyframe(or the first keyframe if not find)
        size_t leftIndex = 0;
        for (size_t i = m_vecKeyFrames.size() - 1; i > 0 ; --i)
        {
            if (m_vecKeyFrames[i].GetPosition() < curPosition) 
            {
                leftIndex = i;
                break;
            }
        }
        KeyFrame& leftKeyFrame = m_vecKeyFrames[leftIndex];

        // take the immediate keyframe after the left keyframe 
        // as the right keyframe(or the last keyframe if the index is overflowed)
        size_t rightIndex = leftIndex + 1;
        KeyFrame& rightKeyFrame = (rightIndex < m_vecKeyFrames.size()) ?
            m_vecKeyFrames[rightIndex] : m_vecKeyFrames[m_vecKeyFrames.size() - 1];

        // when there is just one keyframe, just apply it's value and return
        if (&leftKeyFrame == &rightKeyFrame) 
        {
            SetValueToObject(object, leftKeyFrame.GetValue());
            return;
        }

        // now everything is normal, just do the interpolation
        int leftDistance = curPosition - leftKeyFrame.GetPosition();
        int rightDistance = rightKeyFrame.GetPosition() - curPosition;
        //assert((leftDistance + rightDistance) != 0);

        // alter interpolation position using the right neighbours progression method
        const float interpolationPosition = rightKeyFrame.AlterInterpolationPosition(
            (float)leftDistance / (float)(leftDistance + rightDistance));
        const T result = CInterpolator<T>::InterpolateAbsolute(leftKeyFrame.GetValue(), 
            rightKeyFrame.GetValue(), interpolationPosition);
        
        SetValueToObject(object, result);
    }

    void AddKeyFrame(const KeyFrame& keyFrame)
    {
        m_vecKeyFrames.push_back(keyFrame);
    }

private:
    std::vector<KeyFrame> m_vecKeyFrames;
    virtual void SetValueToObject(CElement* object, const T& value) = 0;
};

class CAlphaAffector : public CAffector<int>
{
private:
    virtual void SetValueToObject(CElement* object, const int& value)
    {
        object->SetAlpha(value, true);
    }

};

class CPositionAffector : public CAffector<CRelVector>
{
private:
    virtual void SetValueToObject(CElement* object, const CRelVector& value)
    {
        object->SetPosition(value, true);
    }
};

class CImageSequenceAffector : public CAffector<StdStringW>
{
private:
    virtual void SetValueToObject(CElement* object, const StdStringW& value)
    {
        (static_cast<CStaticImage*>(object))->SetImage(value, true);
    }
};

}
