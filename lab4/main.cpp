#include <iostream>
#include <utility>
#include <cstdlib>
#include <cstring>
#include <cassert>

template <typename T>
class Heap
{
public:
    // почему пишет ошибку при таком описании 
    // Heap( int capacity ): _capacity(capacity), _size(0), _arr(new T[capacity]) {}
    Heap( int capacity ) {
        _capacity = capacity;
        _size     = 0;
        _arr      = new T[capacity];
    }
    explicit Heap( const T* &array );
    ~Heap();

    void insert( T element );
                                                                                ////
    T extractMin() {
        assert( !isEmpty() );
    
        T tmp = _arr[0];
        _size--;

        if ( !isEmpty () ) {
            _arr[0] = _arr[_size];
            siftDown(0);    
        }
        
        return tmp;
    }

    T peekMin() const {
        assert( !isEmpty() );

        return _arr[0];
    };

    bool isEmpty() const {
        return _size == 0;
    }

    int size() {
        return _size;
    }

    T& operator[]( int i ) {
        assert( 0 <= i && i < _size );

        return _arr[i];
    }

private:
    T  *_arr;
    int _size;
    int _capacity;

    void buildHeap();
    void siftDown( int i );
    void siftUp( int i );
    void resize();
};

// template <typename T>
// Heap<T>::Heap( int capacity ): _capacity(capacity), _size(0), _arr(new T[capacity]) {};

template <typename T>
Heap<T>::Heap( const T* &arr) {
    _size = sizeof( arr ) / sizeof( arr[0] );
    _capacity = _size;
    _arr = new T[_size];

    if ( memcpy( _arr, arr, sizeof( T ) * _size ) != _arr ) {
        throw "error:heap: in constructor. Could not copy array";
    }

    buildHeap();
}

template <typename T>
Heap<T>::~Heap() {
    delete [] _arr;
}

template <typename T>
void Heap<T>::insert( T element ) {
    if ( _size == _capacity ) {
        resize();
    }

    _arr[_size] = element;
    _size++;
    siftUp(_size - 1);
}

template <typename T>
void Heap<T>::resize() {
    T *tmpArr = (T*)realloc(_arr, sizeof(T) * _capacity * 2);
    if ( tmpArr == nullptr ) {
        throw "error:heap: in resize(). Could not resize heap";
    } else {
        _capacity *= 2;
        _arr = tmpArr;
    }
}

template <typename T>
void Heap<T>::buildHeap() {
    for (int i = _size / 2 - 1; i >= 0 ; i--) {
        siftDown( i );
    }
}

template <typename T>
void Heap<T>::siftDown( int i ) {
    // std::cout << "i = " << i << std::endl;
    // std::cout << "size = " << _size << std::endl;
    assert( i >= 0 && i < _size);

    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    int smallest = i;

    if ( left < _size && _arr[i] > _arr[left]) {
        smallest = left;
    }

    if ( right < _size && _arr[i] > _arr[right] ) {
        smallest = right;
    }

    if ( smallest != i ) {
        std::swap( _arr[smallest], _arr[i] );
        siftDown( smallest );
    }
}

template <typename T>
void Heap<T>::siftUp( int i ) {
    assert( i >= 0 && i < _size);

    if ( i == 0 ) {
        return;
    }

    int parent = (i - 1) / 2;

    if ( _arr[parent] > _arr[i] ) {
        std::swap( _arr[parent], _arr[i] );
        siftUp( parent );
    }
}

struct TimeTrainPoint
{
    int arrivalTime = 0;
    int departureTime = 0;

    TimeTrainPoint(int arrival_time, int departure_time): arrivalTime(arrival_time), 
                                                          departureTime(departure_time) {}

    TimeTrainPoint() {}

    bool operator<(const TimeTrainPoint& other) {
        return departureTime < other.departureTime;
    }

    bool operator>(const TimeTrainPoint& other) {
        return departureTime > other.departureTime;
    }
};

void print_trains(Heap<TimeTrainPoint> &trains) {
    for (int i = 0; i < trains.size(); i++) {
        std::cout << trains[i].arrivalTime << " " << trains[i].departureTime << std::endl;
    }
    std::cout << std::endl;
}



int main() {

    int arrivalTime, departureTime;
    int ntrains, maxTrains;
    Heap<TimeTrainPoint> trains(16);

    std::cin >> ntrains;

    maxTrains = 0;
    for (int i = 0; i < ntrains; i++) {
        std::cin >> arrivalTime >> departureTime;

        while ( !trains.isEmpty() && trains.peekMin().departureTime < arrivalTime ) {
            trains.extractMin();
        }

        trains.insert(TimeTrainPoint(arrivalTime, departureTime));

        print_trains(trains);

        if (maxTrains < trains.size()) {
            maxTrains = trains.size();
        }
    }

    std::cout << maxTrains;

    return 0;
}
