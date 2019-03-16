#include <iostream>

int* findCloseElementsIndex(int *array1, int size1, int *array2, int size2) {
    
    int *arrayOfIndex = new int[size2];

    for (int i = 0; i < size2; i++) {
        int end = ( size1 > 2 ) ? 2 : size1;
        int begin = 0;

        while (end != size1 - 1 && array2[i] > array1[end]) {
            begin = end;
            end *= 2;
            end = ( end < size1 - 1 ) ? end : size1 - 1;
        }

        while (end - begin > 1) {
            int middle = (end + begin) / 2;

            if (array1[middle] < array2[i]) {
                begin = middle;
            } else {
                end = middle;
            }
        }

        if ( abs(array2[i] - array1[end]) < abs(array2[i] - array1[begin]) ) {
            arrayOfIndex[i] = end;
        } else {
            arrayOfIndex[i] = begin;
        }
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