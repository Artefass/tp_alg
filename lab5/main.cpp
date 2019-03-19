#include <iostream>
#include <string.h>
#include <functional>
#include <cassert>

template <typename T, typename Comporator>
void Merge(const T *firtsArray, int fLen, const T *secondArray, int sLen, T *mergeArray, Comporator lessThan) {
    
    // вопрос на засыпку, почему валится assert?
    //std::cout << sizeof(mergeArray) << "  " << sizeof(mergeArray[0]) << " elements";
    //assert( (int)(sizeof(mergeArray) / sizeof(mergeArray[0])) >= fLen + sLen );

    int i, j, k;
    i = j = k = 0;
    while (i != fLen && j != sLen) {
        if ( lessThan(firtsArray[i], secondArray[j]) ) {
            mergeArray[k] = firtsArray[i];
            i++;
        } else {
            mergeArray[k] = secondArray[j];
            j++;
        }
        k++;
    }

    if ( i != fLen ) {
        memcpy(mergeArray + k, firtsArray + i, sizeof(T) * (fLen - i));
    } else {
        memcpy(mergeArray + k, secondArray + j, sizeof(T) * (sLen - j));
    }
}

template <typename T, typename Comporator = std::less<T>>
void MergeSort(T *arr, int aLen, Comporator lessThan = Comporator()) {
    
    if (aLen <= 1) {
        return;
    }

    int firstPartArray = aLen / 2;
    int secondPartArray = aLen - firstPartArray;

    MergeSort(arr, firstPartArray);
    MergeSort(arr + firstPartArray, secondPartArray);

    T *mergeArray = new T[aLen];
    
    Merge(arr, firstPartArray, arr + firstPartArray, secondPartArray, mergeArray, lessThan);
    memcpy(arr, mergeArray, sizeof(T) * aLen);
    delete[] mergeArray;
}

struct DataTime
{
    int day = 0;
    int month = 0;
    int year = 0;

    bool operator==(const DataTime &time) const {
        return day == time.day && month == time.month && year == time.year;
    }

    bool operator< (const DataTime &time) const {
        if ( year != time.year ) {
            return year < time.year;
        } else if ( month != time.month ) {
            return month < time.month;
        } else {
            return day < time.day;
        }
    }
};

struct DataTimeMarker
{
    DataTime time = DataTime();
    char event; // имеет два значения ("а" - adulthood (взрослый) и "p" - pensioner (пенсионер))

    DataTimeMarker() {};
    DataTimeMarker(DataTime _time, char _event) : time(_time), event(_event) {};

    bool operator< (const DataTimeMarker &other) const {
        if ( time == other.time ) {
            return event >= other.event;       //  пенсионер важнее совершеннолетнего
        } else {
            return time < other.time;
        }
    }
};

DataTime OldTime(const DataTime &birthday, const DataTime &death) {
    DataTime oldTime(birthday);
    oldTime.year += 80;
    if (oldTime < death) {
        return oldTime;
    } else {
        return death;
    }
}

DataTime AdultTime(const DataTime &birthday) {
    DataTime adultTime(birthday);
    adultTime.year += 18; 
    return adultTime;
}

bool CanAttendMeeting(DataTime &birthday, DataTime &death) {
    DataTime adultTime = AdultTime(birthday);
    return adultTime < death;
}

int FindMaxContemporaries(const DataTimeMarker *arrayDTL, int aLen) {
    
    int maxContemporaries = 0;
    int currentContemporaries = 0;

    for (int i = 0; i < aLen; i++) {
        if ( arrayDTL[i].event == 'a' ) {
            currentContemporaries++;

            if ( maxContemporaries < currentContemporaries ) {
                maxContemporaries = currentContemporaries;
            }
        } else {
            currentContemporaries--;
        }
    }

    return maxContemporaries;
}

int main() {

    int len = 0;
    DataTime birthday, death;
    DataTimeMarker *arrayDTM = nullptr;
    int visitors = 0;

    std::cin >> len;
    arrayDTM = new DataTimeMarker[len * 2];

    for (int i = 0; i < len; i ++) {
        std::cin >> birthday.day >> birthday.month >> birthday.year;
        std::cin >> death.day    >> death.month    >> death.year;

        if ( CanAttendMeeting(birthday, death) ) {
            arrayDTM[2 * visitors]     = DataTimeMarker(AdultTime(birthday), 'a');
            arrayDTM[2 * visitors + 1] = DataTimeMarker(OldTime(birthday, death), 'p');
            visitors++;
        }
    }

    MergeSort(arrayDTM, visitors * 2);
    std:: cout << FindMaxContemporaries(arrayDTM, visitors * 2);

    delete [] arrayDTM;

    return 0;
}