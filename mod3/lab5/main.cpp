#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <math.h>

#include <random>
#include <ctime>

#include "IGraph.h"
#include "CListGraph.h"

//class DistanationFunctor
//{
//    int distanation = 0;
//    size_t start_vertex;
//    size_t end_vertex;
//    bool   init = false;
//public:
//
//    void operator()(const size_t vertex, const int edge)
//    {
//        if ( !init ) {
//            start_vertex = vertex;
//            init = true;
//        }
//
//        distanation += edge;
//    }
//
//    int GetDistanation(){
//        if ( init )
//
//        return distanation;
//    }
//
//};

std::vector<std::pair<int, int>> BoxMullerGeneratePoints( size_t nPoints ) {
    assert( nPoints > 0 );
    double s;
    double x, y;
    int    ix, iy;

    std::vector<std::pair<int, int>> points(nPoints);

    srand( time( 0 ) ); // автоматическая рандомизация

    for ( auto i = 0; i < nPoints; i++) {
        do {
            x = ((double) rand()) / RAND_MAX;
            y = ((double) rand()) / RAND_MAX;

            s = x*x + y*y;
        } while (s <= 0 || s > 1);

        ix = (int)(x * RAND_MAX);
        iy = (int)(y * RAND_MAX);

        points.emplace_back(std::make_pair(ix, iy));
    }

    return points;
}

double calcDistance(const std::pair<int, int> &lpoint, const std::pair<int, int> &rpoint) {
    return std::sqrt(
            (rpoint.first - lpoint.first) * (rpoint.first - lpoint.first) +
            (rpoint.second - lpoint.second) * (rpoint.second - lpoint.second));
}

int main() {
    size_t nvertices;
    size_t from, to;
    int    edge;

    std::cin >> nvertices;
    CListGraph graph(nvertices);

    std::vector<std::pair<int, int>> random_points;
    random_points = BoxMullerGeneratePoints(nvertices);

    for ( size_t i = 0; i < nvertices; i++ )
        std::cout << "(" << random_points[i].first << "," << random_points[i].second << ") ";
    std::cout << std::endl;

    for (size_t i = 0; i < nvertices; i++)
        for ( size_t j = i + 1; j < nvertices; j++ ){
            double distance = calcDistance(random_points[i], random_points[j]);
            graph.AddEdge(i,j,distance);
            graph.AddEdge(j,i,distance);
        }

    CListGraph MST(nvertices);
    Prim(graph, MST, 0);

   // BFS(MST, 0, [](size_t vertex, int edge){ MST_weight += edge; });

    return 0;
}