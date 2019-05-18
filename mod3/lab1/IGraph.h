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
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

IGraph::~IGraph() {}


void BFS(const IGraph &graph, int vertex, void (*func)(int))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> next_vertices;

    next_vertices.push(vertex);
    visited[vertex] = true;

    while (next_vertices.size())
    {
        int current = next_vertices.front();
        next_vertices.pop();

        func(current);

        for (int next_vertex: graph.GetNextVertices(current))
        {
            if (!visited[next_vertex])
            {
                visited[next_vertex] = true;
                next_vertices.push(next_vertex);
            }
        }
    }
}

void DFS(const IGraph &graph, int vertex, void (*func)(int))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::stack<int> next_vertices;

    next_vertices.push(vertex);
    visited[vertex] = true;

    while (next_vertices.size())
    {
        int current = next_vertices.top();
        next_vertices.pop();

        func(current);

        for (int next_vertex: graph.GetNextVertices(current))
        {
            if (!visited[next_vertex])
            {
                visited[next_vertex] = true;
                next_vertices.push(next_vertex);
            }
        }
    }
}

#endif //LAB1_IGRAPH_H
