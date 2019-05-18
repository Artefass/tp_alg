#include <iostream>

#include "IGraph.h"
#include "CListGraph.h"
#include "CMatrixGraph.h"
#include "CSetGraph.h"
#include "CArcGraph.h"

int main(int argc, const char * argv[]) {
    CListGraph   graph(7);
//    CMatrixGraph mgraph(graph);
//    CSetGraph    graph(7);
//    CArcGraph    graph(7);

    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);

    CMatrixGraph mgraph(graph);
    CArcGraph    agraph(mgraph);
    CSetGraph    sgraph(agraph);
    CListGraph   lgraph(sgraph);

    BFS(graph, 0, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    BFS(mgraph, 0, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    BFS(agraph, 0, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    BFS(sgraph, 0, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    BFS(lgraph, 0, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

//    DFS(graph, 0, [](int vertex){ std::cout << vertex << " "; });
//    std::cout << std::endl;

    return 0;
}