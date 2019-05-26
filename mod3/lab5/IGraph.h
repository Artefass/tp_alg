//
// Created by pasha on 18.05.2019.
//

#ifndef LAB1_IGRAPH_H
#define LAB1_IGRAPH_H

#include <vector>
#include <queue>
#include <stack>

struct IGraph {
    virtual ~IGraph() = 0;

    // Добавление ребра от from к to.
    virtual void AddEdge(size_t from, size_t to, int weight) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<std::pair<size_t, int>> GetNextVertices(size_t vertex) const = 0;
    virtual std::vector<std::pair<size_t, int>> GetPrevVertices(size_t vertex) const = 0;
};

IGraph::~IGraph() {}

void BFS(const IGraph &graph, size_t vertex, void (*func)(size_t, int))
{
    std::vector<bool>  visited(graph.VerticesCount(), false);
    std::queue<std::pair<size_t, int>> next_vertices;

    next_vertices.push(std::make_pair(vertex, 0));
    visited[vertex] = true;

    while ( !next_vertices.empty() )
    {
        std::pair<size_t, int> ve = next_vertices.front();
        next_vertices.pop();

        size_t v      = ve.first;
        int    v_edge = ve.second;

        func(v, v_edge);

        for (auto ue: graph.GetNextVertices(v))
        {
            size_t u      = ue.first;
            int    u_edge = ue.second;

            if ( !visited[u] )
            {
                visited[u] = true;
                next_vertices.push(std::make_pair(u, u_edge));
            }
        }
    }
}

#endif //LAB1_IGRAPH_H


