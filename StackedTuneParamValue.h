#pragma once

#include <QVariant>
#include <qmath.h>
#include <QDebug>

template<typename T>
struct RangedValue
{
    //explicit RangedValue() = default;

    explicit RangedValue(const T& min = std::numeric_limits<T>::lowest(),
                         const T& max = std::numeric_limits<T>::max())
    {
        if (min <= max) {
            this->min = min;
            this->max = max;
        }
    }

    T value() const
    {
        return val;
    }

    T minimum() const
    {
        return min;
    }

    T maximum() const
    {
        return max;
    }

protected:

    T val, min, max;

};

//template<typename T>
//using MinRangedValue = RangedValue<T>(1, 0);

template<typename T>
struct MinRangedValue : public RangedValue<T>
{
    explicit MinRangedValue(const T& min):
        RangedValue<T>(min, std::numeric_limits<T>::max())
    {
    }

};

template<typename T>
struct MaxRangedValue : public RangedValue<T>
{
    explicit MaxRangedValue(const T& max):
        RangedValue<T>(std::numeric_limits<T>::lowest(), max)
    {
    }

};

class StackedTuneParamValue : public QStringList
{
public:

    explicit StackedTuneParamValue() = default;

    explicit StackedTuneParamValue(const QStringList& values, int i = 0):
        QStringList(values)
    { index = (i >= 0 && i < size()) ? i : isEmpty() ? -1 : 0; }

    bool setIndex(int i)
    {
        std::numeric_limits<float>::quiet_NaN();
        if (i >= 0 && i < size()) {
            index = i;
            return true;
        }

        return false;
    }

    int currentIndex() const
    {
        return index;
    }

    QString text() const
    {
        return at(index);
    }

private:

    int index{-1};

};

Q_DECLARE_METATYPE(RangedValue<int>)
Q_DECLARE_METATYPE(RangedValue<float>)
Q_DECLARE_METATYPE(StackedTuneParamValue)
