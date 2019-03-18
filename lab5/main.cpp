#include <iostream>
#include <string.h>
#include <functional>
#include <cassert>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

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

    DataTime operator-(DataTime &other) {
        DataTime diff(*this);

        diff.day -= other.day;
        while ( diff.day < 0 ) {
            diff.month--;
            diff.day += diff.daysInMonth();
        }

        diff.month -= other.month;
        while ( diff.month < 0) {
            diff.year--;
            diff.month += 12;
        }

        diff.year -= other.year;

        return diff;
    }

    int daysInMonth() {
        return daysInMonth(month);
    }

    int daysInMonth(int aMonth) {
        int days = 0;

        if ( aMonth == 2 ) {
            if ( year % 4 == 0 && year % 100 != 0 ) {
                days = 29;
            } else if ( year % 400 == 0 ) {
                days = 29;
            } else {
                days = 28;
            }
        } else {
            days = ( ( aMonth % 8 + aMonth / 8 ) % 2 == 0 ) ? 30 : 31;
        }

        return days;
    }

    int allDays() {
        int days = 0;

        days += year * 365;
        
        for (int i = 1; i <= month; i++) {
            days += daysInMonth(i);
        }

        days += day;

        return days;
    }

    // уменьшаем дату на день
    DataTime& operator--() {
        
        if ( day > 1 ){
            day--;
        } else if ( month > 1 ) {
            month--;
            day = this->daysInMonth();
        } else {
            day = 31;
            month = 12;
            year--;
        }

        return *this;
    }

    DataTime operator--(int) {
        
        DataTime tmp(*this);
        operator--();
        return tmp;
    }

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

    void updateDataTimeMarker(DataTime aTime, char aEvent) {
        time = aTime;
        event = aEvent;
    }

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
    //oldTime--;
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
    
    //std::cout << "day: " << birthday.day << " " << birthday.month << " " << birthday.year << std::endl;

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
    int peoples = 0;

    std::cin >> len;
    arrayDTM = new DataTimeMarker[len * 2];

    for (int i = 0; i < len; i ++) {
        std::cin >> birthday.day >> birthday.month >> birthday.year;
        std::cin >> death.day    >> death.month    >> death.year;

        //std::cout << "Birthday: " << birthday.day << " " << birthday.month << " " << birthday.year << std::endl;
        //std::cout << "Death: " << death.day << " " << death.month << " " << death.year << std::endl;

        //DataTimeLife diff = death - birthday;
        //std::cout << "Diff: " << diff.day << " " << diff.month << " " << diff.year << std::endl;

        if ( CanAttendMeeting(birthday, death) ) {
            arrayDTM[2 * peoples].updateDataTimeMarker(AdultTime(birthday), 'a'); // есть баг с високосным годом
            arrayDTM[2 * peoples + 1].updateDataTimeMarker(OldTime(birthday, death), 'p');
            peoples++;
        }
    }

    MergeSort(arrayDTM, peoples * 2);
    // for (int i = 0; i < peoples * 2; i++) {
    //     std::cout << "[" << i << "]" <<" marker " << arrayDTM[i].event << " day: " << arrayDTM[i].time.day 
    //               << " month: " << arrayDTM[i].time.month << " year: " << arrayDTM[i].time.year << std::endl;  
    // }
    std:: cout << FindMaxContemporaries(arrayDTM, peoples * 2);

    delete[] arrayDTM;

    return 0;
}