#pragma once

#include <math.h>

namespace Meepo
{

enum ProgressionMode
{
    ProgressionMode_Linear,					//! linear progression
    ProgressionMode_Quadratic_Accelerating,	//! progress is accelerated, starts slow and speeds up
    ProgressionMode_Quadratic_Decelerating,	//! progress is decelerated, starts fast and slows down
	ProgressionMode_Discrete                /** left value is picked if interpolation position is lower than 1.0,
									        *   right is only picked when interpolation position is exactly 1.0
									        */
};

template <class T>
class CKeyFrame
{
public:


    CKeyFrame(int position, const T& value, ProgressionMode progressionMode = ProgressionMode_Linear)
        : m_position(position), m_value(value), m_progressionMode(progressionMode)
    {}

    int GetPosition() const { return m_position; }
    const T& GetValue() const { return m_value; }
    void SetProgressionMode(ProgressionMode mode) { m_progressionMode = mode; }

    float AlterInterpolationPosition(float position)
    {
        switch (m_progressionMode)
        {
        case ProgressionMode_Linear:
            return position;
        case ProgressionMode_Quadratic_Accelerating:
            return position * position;
        case ProgressionMode_Quadratic_Decelerating:
            return sqrtf(position);
        case ProgressionMode_Discrete:
            return position < 1.0f ? 0.0f : 1.0f;
        }

        // todo: more progression methods?
        return position;
    }

private:
    int m_position;
    T m_value;
    ProgressionMode m_progressionMode;
};

}