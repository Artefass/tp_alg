//
// Created by pasha on 18.05.2019.
//

#ifndef LAB1_CARCGRAPH_H
#define LAB1_CARCGRAPH_H

#include <iostream>
#include <vector>
#include <cassert>

#include "IGraph.h"

class CArcGraph: public IGraph
{
public:
    CArcGraph(int n): vertexCount(n) {}
    CArcGraph(const IGraph &graph)
    {
        adjacency_lists.clear();
        vertexCount = graph.VerticesCount();
        for (int from = 0; from < vertexCount; from++)
        {
            for (int to: graph.GetNextVertices((from)))
                adjacency_lists.emplace_back(std::make_pair(from, to));
        }
    }

    ~CArcGraph() {}

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < vertexCount);
        assert(0 <= to && to < vertexCount);
        adjacency_lists.emplace_back(std::make_pair(from, to));
    }

    int VerticesCount() const override
    {
        return vertexCount;
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < vertexCount);

        std::vector<int> next_vertices;

        for ( const std::pair<int, int> &pair: adjacency_lists )
            if ( pair.first == vertex )
                next_vertices.push_back(pair.second);

        return next_vertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < vertexCount);
        std::vector<int> prev_vertices;
        for ( const std::pair<int, int> &pair: adjacency_lists )
            if ( pair.second == vertex )
                prev_vertices.push_back(pair.first);
        return prev_vertices;
    }

private:
    int vertexCount = 0;
    std::vector<std::pair<int,int>> adjacency_lists;
};


#endif //LAB1_CARCGRAPH_H
