#include <utility>
#include <iostream>


void Partition(unsigned long long *arr, int size, unsigned long long mask) {
    //std::cout << mask << std::endl;

    if ( mask == 0 || size == 0 ) {
        return;
    }

    int i,j;

    for (i = 0, j = 0; j < size; j++) {
        if ( (arr[j] & mask) == 0 ) {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }

    Partition(    arr,        i, mask >> 1);
    Partition(arr + i, size - i, mask >> 1);
}

void BinarySort(unsigned long long *arr, int size) {

    int  bits = sizeof(unsigned long long) * 8 - 1;

    //std::cout << "bits: " << bits << std::endl;
    unsigned long long mask = 0x1;

    while ( bits ) {
        mask <<= 1;
        bits--;
    }

    //std::cout << "mask:" << mask << std::endl; 

    //std::cout << "bits: " << bits << std::endl;

    Partition(arr, size, mask);
}

int main() {

    int n;
    unsigned long long *arr;

    std::cin >> n;
    arr = new unsigned long long[n];

    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    BinarySort(arr, n);

    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }

    delete [] arr;

    return 0;
}