//
// Created by pasha on 18.05.2019.
//

#ifndef LAB2_CLISTGRAPH_H
#define LAB2_CLISTGRAPH_H

#include <vector>
#include <cassert>

#include "IGraph.h"

class CListGraph: public IGraph
{
public:
    CListGraph(int n) : adjacency_lists(n) {}
    CListGraph(const IGraph &graph)
    {
        adjacency_lists.resize(graph.VerticesCount());
        for (int i = 0; i < adjacency_lists.size(); i++)
        {
            adjacency_lists[i] = graph.GetNextVertices(i);
        }
    }
    ~CListGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacency_lists.size());
        assert(0 <= to && to < adjacency_lists.size());
        adjacency_lists[from].push_back(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacency_lists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacency_lists.size());
        return adjacency_lists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev_vertices;

        for (int from = 0; from < adjacency_lists.size(); from++)
        {
            for (int to: adjacency_lists[from])
            {
                if (to == vertex)
                {
                    prev_vertices.push_back(from);
                }
            }
        }

        return prev_vertices;
    }

private:
    std::vector<std::vector<int>> adjacency_lists;
};

#endif //LAB2_CLISTGRAPH_H
