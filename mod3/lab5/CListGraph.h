#include <iostream>
#include <vector>
#include <cassert>

#include "IGraph.h"

class CListGraph: public IGraph
{
public:
    explicit CListGraph(size_t n) : adjacency_lists(n) {}
    CListGraph(const IGraph &graph)
    {
        adjacency_lists.resize(graph.VerticesCount());
        for (int i = 0; i < adjacency_lists.size(); i++)
        {
            adjacency_lists[i] = graph.GetNextVertices(i);
        }
    }

    ~CListGraph() {}
    
    void AddEdge(size_t from, size_t to, int weight) override
    {
        assert(0 <= from && from < adjacency_lists.size());
        assert(0 <= to && to < adjacency_lists.size());
        adjacency_lists[from].emplace_back(std::make_pair(to, weight));
    }

    int VerticesCount() const override
    {
        return (int)adjacency_lists.size();
    }
    
    std::vector<std::pair<size_t, int>> GetNextVertices(size_t vertex) const override
    {
        assert(0 <= vertex && vertex < adjacency_lists.size());
        return adjacency_lists[vertex];
    }
    
    std::vector<std::pair<size_t, int>> GetPrevVertices(size_t vertex) const override
    {
        std::vector<std::pair<size_t, int>> prev_vertices;
        
        for (int from = 0; from < adjacency_lists.size(); from++)
        {
            for (std::pair<size_t, int > to: adjacency_lists[from])
            {
                if (to.first == vertex)
                {
                    prev_vertices.emplace_back(std::make_pair(from, to.second));
                }
            }
        }
        
        return prev_vertices;
    }
    
private:
    std::vector<std::vector<std::pair<size_t, int>>> adjacency_lists;
};
