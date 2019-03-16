#include <iostream>
#include <cstdlib>
#include <cassert>

#define BASE_SIZE 16

struct Point2D {
    Point2D(){};
    Point2D(int x, int y) : x(x), y(y) {};
    int x,y;
};

class ConvexPolygon
{
    Point2D *arrayPoints = nullptr;
    int capacity = 0;
    int size = 0;

public:
    ConvexPolygon() : capacity(BASE_SIZE) {
        arrayPoints = new Point2D[capacity];
    }

    ~ConvexPolygon() {
        delete [] arrayPoints;
    }

    void addPoint(Point2D point) {
        if ( size == capacity ) {
            Point2D *tmpArray = (Point2D*)realloc(arrayPoints, sizeof(Point2D) * capacity * 2);
            if ( tmpArray == nullptr ) {
                throw "ConvexPolygon error: could not allocate memmory";    
            }
            
            capacity *= 2;
            arrayPoints = tmpArray;
        }

        arrayPoints[size] = point;
        size++;
    }

    double calculateArea() {
        assert(size > 2);

        double result = 0;

        for (int i = 0; i < size - 1; i++) {
            Point2D a = arrayPoints[i];
            Point2D b = arrayPoints[i + 1];

            result += (b.x - a.x) * (b.y + a.y) / 2.0;     
        }

        return -result;
    }
};

int main(){
    int nPoints = 0;
    int x,y;
    Point2D point;
    ConvexPolygon cp;

    std::cin >> nPoints;

    for (int i = 0; i < nPoints; i++) {
        std::cin >> x >> y;
        cp.addPoint(Point2D(x,y));
    }

    std::cout << cp.calculateArea();

    return 0;
}