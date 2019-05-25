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


void BFS(const IGraph &graph, size_t vertex, void (*func)(size_t))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<size_t> next_vertices;

    next_vertices.push(vertex);
    visited[vertex] = true;

    while (next_vertices.size())
    {
        int current = next_vertices.front();
        next_vertices.pop();

        func(current);

        for (auto next_vertex: graph.GetNextVertices(current))
        {
            if (!visited[next_vertex.first])
            {
                visited[next_vertex.first] = true;
                next_vertices.push(next_vertex.first);
            }
        }
    }
}

#endif //LAB1_IGRAPH_H
