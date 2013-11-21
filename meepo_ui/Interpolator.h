#pragma once

#include "StringHelper.h"

namespace Meepo
{
   
template <class T>
class CInterpolator
{
public:
    static T InterpolateAbsolute(const T& value1, const T& value2, float position);
    //T InterpolateRelative(const T& base, const T& value1, const T& value2, float position);
    //T InterpolateRelativeMultiply(const T& base, const T& value1, const T& value2, float position);
};

template<>
class CInterpolator<int>
{
public:
    static int InterpolateAbsolute(const int& val1, const int& val2, float position)
    {
        const int result = static_cast<int>(
            static_cast<float>(val1) * (1.0f - position) +
            static_cast<float>(val2) * (position)
            );
        return result;
    }
};

template<>
class CInterpolator<CRelVector>
{
public:
    static CRelVector InterpolateAbsolute(const CRelVector& val1, const CRelVector& val2, float position)
    {
        const CRelVector result =
            val1 * CRelValue(1.0f - position, 1.0f - position) +
            val2 * CRelValue(position, position);
        return result;
    }
};

template<>
class CInterpolator<std::wstring>
{
public:
    static StdStringW InterpolateAbsolute(const StdStringW& val1, const StdStringW& val2, float position)
    {
        size_t numberPos = val1.find_last_not_of(L"0123456789") + 1;
        StdStringW prefix = val1.substr(0, numberPos);

        const int ival1 = StringHelper::StringToInt(val1.substr(numberPos));
        const int ival2 = StringHelper::StringToInt(val2);

        const int result = static_cast<int>(
            static_cast<float>(ival1) * (1.0f - position) +
            static_cast<float>(ival2) * (position)
            ); 

        std::wstring final = prefix + StringHelper::IntToString(result);

        return final;
    }
};

} 