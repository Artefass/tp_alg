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

struct DataTimeMarker
{
    int day;
    int month;
    int year;
    char event; // имеет два значения ("а" - adulthood (взрослый) и "p" - pensioner (пенсионер))

    DataTimeLife() : day(0), month(0), year(0) {}

    DataTimeLife(int aDay, int aMonth, int aYear, int aEvent) : day(aDay), month(aMonth), year(aYear) {

        assert( aEvent == 'a' || aEvent == 'p' );

        event = aEvent;
    }

    void update(int aDay, int aMonth, int aYear, char aEvent) {
        day   = aDay;
        month = aMonth;
        year  = aYear;
        event = aEvent;
    }

    DataTimeLife operator- (DataTimeLife &other) {
        DataTimeLife diff(*this);

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

        int days = 0;

        if ( month == 2 ) {
            if ( year % 4 == 0 && year % 100 != 0 ) {
                days = 29;
            } else if ( year % 400 == 0 ) {
                days = 29;
            } else {
                days = 28;
            }
        } else {
            days = ( ( month % 8 + month / 8 ) % 2 == 0 ) ? 30 : 31;
        }

        return days;
    }

    // уменьшаем дату на день
    DataTimeLife& operator--() {
        
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

    DataTimeLife operator--(int) {
        
        DataTimeLife tmp(*this);
        operator--();
        return tmp;
    }

    bool operator< (const DataTimeLife &data) const {
        if ( year != data.year ) {
            return year < data.year;
        } else if ( month != data.month ) {
            return month < data.month;
        } else if ( day != data.day ) {
            return day < data.day;
        } else {
            return !(event <= data.event);             // работает с текущей структурайб т.к. значение 
                                                    // символа 'a' меньше значения символа 'p' 
        }
    }
};

bool CanAttendMeeting(DataTimeLife &birthday, DataTimeLife &death) {
    DataTimeLife timeLife = death - birthday;
    
    return timeLife.year >= 18;
}

int FindMaxContemporaries(const DataTimeLife *arrayDTL, int aLen) {
    
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
    DataTimeLife birthday, death;
    DataTimeLife *arrayDTL = nullptr;
    int visitors = 0;

    std::cin >> len;
    arrayDTL = new DataTimeLife[len * 2];

    for (int i = 0; i < len; i ++) {
        std::cin >> birthday.day >> birthday.month >> birthday.year;
        std::cin >> death.day    >> death.month    >> death.year;

        //std::cout << "Birthday: " << birthday.day << " " << birthday.month << " " << birthday.year << std::endl;
        //std::cout << "Death: " << death.day << " " << death.month << " " << death.year << std::endl;

        //DataTimeLife diff = death - birthday;
        //std::cout << "Diff: " << diff.day << " " << diff.month << " " << diff.year << std::endl;

        if ( CanAttendMeeting(birthday, death) ) {
            visitors++;
            arrayDTL[2 * i].update(birthday.day, birthday.month, birthday.year + 18, 'a'); // есть баг с високосным годом
            arrayDTL[2 * i + 1].update(death.day, death.month, min(birthday.year + 80, death.year), 'p');
        }
    }

    MergeSort(arrayDTL, visitors * 2);
    std:: cout << FindMaxContemporaries(arrayDTL, visitors * 2);

    delete[] arrayDTL;

    return 0;
}