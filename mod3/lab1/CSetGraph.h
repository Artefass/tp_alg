//
// Created by pasha on 18.05.2019.
//

#ifndef LAB1_CSETGRAPH_H
#define LAB1_CSETGRAPH_H

#include <unordered_set>
#include <vector>
#include <cassert>

#include "IGraph.h"

class CSetGraph: public IGraph
{
public:
    CSetGraph(int n): adjacency_lists(n){}
    CSetGraph(const IGraph &graph)
    {
        adjacency_lists.resize(graph.VerticesCount());
        for (int i = 0; i < adjacency_lists.size(); i++)
        {
            for (int j: graph.GetNextVertices(i)){
                adjacency_lists[i].insert(j);
            }
        }
    }

    ~CSetGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacency_lists.size());
        assert(0 <= to && to < adjacency_lists.size());
        adjacency_lists[from].insert(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacency_lists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacency_lists.size());

        std::vector<int> nextVertices;

        for (const int &to: adjacency_lists[vertex])
            nextVertices.push_back(to);

        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prev_vertices;

        for (int from = 0; from < adjacency_lists.size(); from++)
        {
            const std::unordered_set<int> &fromVertexSet = adjacency_lists[from];
            if ( fromVertexSet.find(vertex) != fromVertexSet.end() )
                prev_vertices.push_back(from);
        }

        return prev_vertices;
    }
private:
    std::vector<std::unordered_set<int>> adjacency_lists;
};


#endif //LAB1_CSETGRAPH_H
