#include <utility>
#include <iostream>
#include <algorithm>

using namespace std;

int median3Index(const int *arr, int i, int j, int k) {
    if ( arr[i] > arr[j] ) {
        if ( arr[j] > arr[k] ) {
            return j;
        } else if ( arr[i] > arr[k] ) {
            return k;
        } else {
            return i;
        }
    } else {
        if ( arr[i] > arr[k] ) {
            return i;
        } else if ( arr[j] > arr[k] ) {
            return k;
        } else {
            return j;
        }
    }
}

int partition( int *arr, int size, int k) {
    
    int i,j;
    int middle;

    int start = 0;
    int end   = size - 1;

    while ( end - start > 0 ) {
        middle = median3Index(arr, start, (end + start) / 2, end);
        std::swap(arr[middle], arr[start]);

        for ( i = end, j = end; j > start; j-- ) {
            if ( arr[j] >= arr[start] ) {
                std::swap(arr[j], arr[i]);
                i--;
            }
        }

        std::swap(arr[i], arr[start]);

        if ( k == i ) {
            break;
        } else if ( k > i ) {
            start = i + 1;
        } else {
            end   = i - 1;
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