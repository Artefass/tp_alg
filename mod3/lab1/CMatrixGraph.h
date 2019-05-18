//
// Created by pasha on 18.05.2019.
//

#ifndef LAB1_CMATRIXGRAPH_H
#define LAB1_CMATRIXGRAPH_H

#include <vector>
#include <cassert>

#include "IGraph.h"

class CMatrixGraph: public IGraph
{
public:
    CMatrixGraph(int n): adjacency_lists(n, std::vector<int>(n, 0)){}
    CMatrixGraph(const IGraph &graph)
    {
        adjacency_lists.resize(graph.VerticesCount());
        for (std::vector<int> &vector: adjacency_lists)
            vector.resize(graph.VerticesCount());

        for (int i = 0; i < adjacency_lists.size(); i++)
        {
            for (int &j: graph.GetNextVertices(i)){
                adjacency_lists[i][j] = 1;
            }
        }
    }

    ~CMatrixGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacency_lists.size());
        assert(0 <= to && to < adjacency_lists.size());
        adjacency_lists[from][to] = 1;
    }

    int VerticesCount() const override
    {
        return (int)adjacency_lists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacency_lists.size());

        std::vector<int> nextVertices;

        for (int to = 0; to < adjacency_lists.size(); to++) {
            if ( adjacency_lists[vertex][to] == 1 )
                nextVertices.push_back(to);
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev_vertices;

        for (int from = 0; from < adjacency_lists.size(); from++)
        {
            if ( adjacency_lists[from][vertex] == 1 )
                prev_vertices.push_back(from);
        }

        return prev_vertices;
    }
private:
    std::vector<std::vector<int>> adjacency_lists;
};

#endif //LAB1_CMATRIXGRAPH_H
