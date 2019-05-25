#include <iostream>
#include <vector>

#include "CListGraph.h"

const int INFINITE = 50000;

struct GreaterEdge;

typedef std::priority_queue<std::pair<size_t, int>, std::vector<std::pair<size_t, int>>, GreaterEdge>
        priority_queue_on_graph;

struct GreaterEdge
{
    bool operator()(std::pair<size_t, int> &left, std::pair<size_t, int> &right){
        return left.second > right.second;
    }
};

int SmallestDistance(const IGraph &graph, size_t from, size_t to)
{
    std::vector<int>  distance(graph.VerticesCount(), INFINITE);
    std::vector<bool> visited(graph.VerticesCount(), false);
    priority_queue_on_graph queue;

    distance[from] = 0;
    queue.emplace(std::make_pair(from, 0));

    while ( !queue.empty() )
    {
        std::pair<size_t, int> min_element = queue.top();
        size_t u = min_element.first;

        queue.pop();

        if ( !visited[u] )
        {
            visited[u] = true;
            for ( std::pair<size_t, int> &element: graph.GetNextVertices(u))
            {
                size_t v = element.first;
                int edge = element.second;

                if ( !visited[v] )
                    if ( distance[v] > distance[u] + edge ){
                        distance[v] = distance[u] + edge;
                        queue.emplace(std::make_pair(v, distance[v]));
                    }
            }
        }
    }

    return distance[to];
}

int main() {
    size_t nvertex;
    size_t nedges;
    size_t from, to;
    int edge;

    std::cin >> nvertex >> nedges;

//    nvertex = 6;
//    nedges  = 9;

    CListGraph graph(nvertex);

    for ( auto i = 0; i < nedges; i++ )
    {
        std::cin >> from >> to >> edge;
        graph.AddEdge(from, to, edge);
        graph.AddEdge(to, from, edge);
    }

    std::cin >> from >> to;

//    graph.AddEdge(0, 3, 1);
//    graph.AddEdge(3, 0, 1);
//    graph.AddEdge(0, 4, 2);
//    graph.AddEdge(4, 0, 2);
//    graph.AddEdge(1, 2, 7);
//    graph.AddEdge(2, 1, 7);
//    graph.AddEdge(1, 3, 2);
//    graph.AddEdge(3, 1, 2);
//    graph.AddEdge(1, 4, 3);
//    graph.AddEdge(4, 1, 3);
//    graph.AddEdge(1, 5, 3);
//    graph.AddEdge(5, 1, 3);
//    graph.AddEdge(2, 5, 3);
//    graph.AddEdge(5, 2, 3);
//    graph.AddEdge(3, 4, 4);
//    graph.AddEdge(4, 3, 4);
//    graph.AddEdge(3, 5, 6);
//    graph.AddEdge(5, 3, 6);

//    from = 0;
//    to = 2;

//    BFS(graph, 0, [](size_t vertex){ std::cout << vertex << " "; });
//    std::cout << std::endl;
//
    std::cout << SmallestDistance(graph, from, to);
    return 0;
}