#include <iostream>

void findInterval(int *array, int size, int element, int &begin, int &end) {

    while (end != size - 1 && element > array[end]) {
        begin = end;
        end *= 2;
        end = ( end < size - 1 ) ? end : size - 1;
    }
}

void findPlaceBetweenTwoNearestElements(int *array, int element, int &firstIndex, int &secondIndex) {

    while (secondIndex - firstIndex > 1) {
        int middle = (firstIndex + secondIndex) / 2;

        if (array[middle] < element) {
            firstIndex = middle;
        } else {
            secondIndex = middle;
        }
    }
}

int chooseNearestBetwenTwoIndex(int *array, int element, int firstIndex, int secondIndex) {

    if ( abs(element - array[secondIndex]) < abs(element - array[firstIndex]) ) {
        return secondIndex;
    } else {
        return firstIndex;
    }
}

int* findCloseElementsIndex(int *array1, int size1, int *array2, int size2) {
    
    int *arrayOfIndex = new int[size2];

    for (int i = 0; i < size2; i++) {
        int end = ( size1 > 2 ) ? 2 : size1;
        int begin = 0;

        findInterval(array1, size1, array2[i], begin, end);
        findPlaceBetweenTwoNearestElements(array1, array2[i], begin, end);
        arrayOfIndex[i] = chooseNearestBetwenTwoIndex(array1, array2[i], begin, end);
    }

    return arrayOfIndex;
}

int main() {
    int *buff1 = nullptr;
    int *buff2 = nullptr;
    int buffSize1 = 0;
    int buffSize2 = 0;

    std::cin >> buffSize1;
    buff1 = new int[buffSize1];

    for (int i = 0; i < buffSize1; i++) {
        std::cin >> buff1[i];
    }

    std::cin >> buffSize2;
    buff2 = new int[buffSize2];

    for (int i = 0; i < buffSize2; i++) {
        std::cin >> buff2[i];
    }

    int *arrayOfIndex = findCloseElementsIndex(buff1, buffSize1, buff2, buffSize2);

    for (int i = 0; i < buffSize2; i++) {
        std::cout << arrayOfIndex[i] << " ";
    }

    delete [] buff1;
    delete [] buff2;
    delete [] arrayOfIndex;

    return 0;
}