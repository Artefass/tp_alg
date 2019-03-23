#include <utility>
#include <iostream>

int partition(int *arr, int size, int k) {
    
    int i,j;

    int start = 0;
    int end   = size - 1;

    while ( end - start > 0 ) {
        int middle = (arr[start] + arr[(end + start) / 2] + arr[end]) / 3;
        for ( i = end, j = end; j >= start; j-- ) {
            if ( arr[j] > middle ) {
                std::swap(arr[j], arr[i]);
                i--;
            }
        }

        if ( k > i ) {
            start = i + 1;
        } else {
            end   = i;
        }
    }

    return arr[k];
}

int main() {
    
    int n,k;
    int *arr;

    std::cin >> n >> k;
    arr = new int[n];

    for(int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    std::cout << partition(arr, n, k);

    delete [] arr;

    return 0;
}