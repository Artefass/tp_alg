#include <iostream>
#include <algorithm>
#include <queue>

#include "CListGraph.h"

const int INFINITE = 100;

int FindAllSmallestWays(IGraph &graph, const int &from, const int &to) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int>   queue;

    std::vector<int>  distance(graph.VerticesCount(), INFINITE);
    std::vector<int>  ways(graph.VerticesCount(), 0);

    queue.push(from);
    visited[from] = true;
    distance[from] = 0;
    ways[from] = 1;

    while ( !queue.empty() )
    {
        int current = queue.front();
        queue.pop();

        for (int next_vertex: graph.GetNextVertices(current))
        {
            if ( !visited[next_vertex] )
            {
                queue.push(next_vertex);
                visited[next_vertex] = true;
            }

            if ( distance[current] + 1 == distance[next_vertex] )
                ways[next_vertex] += ways[current];
            else if ( distance[current] + 1 < distance[next_vertex] )
            {
                distance[next_vertex] = distance[current] + 1;
                ways[next_vertex] = ways[current];
            }
        }
    }

    return ways[to];
}

int main() {
    int nvertex;
    int nedge;
    int from, to;

    std::cin >> nvertex;
    std::cin >> nedge;

    CListGraph graph(nvertex);

    for ( size_t i = 0; i < nedge; i++ )
    {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }

    // вершины, через которые нужно найти пути
    std::cin >> from >> to;

    std::cout << FindAllSmallestWays(graph, from, to);

    return 0;
}