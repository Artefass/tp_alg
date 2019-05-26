#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <climits>

#include "IGraph.h"
#include "CListGraph.h"

struct GreaterEdge;

typedef std::pair<size_t, int> vertex_edge_t;
typedef std::priority_queue<vertex_edge_t, std::vector<vertex_edge_t>,GreaterEdge> Queue;

const int INFINITE = INT_MAX;

struct GreaterEdge {
    bool operator()(const vertex_edge_t &left, const vertex_edge_t &right)
    {
        return left.second > right.second;
    }
};

void Prim(const IGraph &graph, IGraph &MST, size_t from)
{
    assert( graph.VerticesCount() == MST.VerticesCount() );
    assert( 0 <= from && from < graph.VerticesCount() );

    std::vector<int>    min_e(graph.VerticesCount(), INFINITE);
    std::vector<size_t> parent(graph.VerticesCount(), -1);
    Queue queue;

    queue.emplace(std::make_pair(from, 0));
    min_e[from] = 0;

    while ( !queue.empty() )
    {
        vertex_edge_t ve = queue.top();
        queue.pop();

        size_t v      = ve.first;
        int    v_edge = ve.second;

        if ( min_e[v] < v_edge )
            continue;

        if ( v != from )
        {
            MST.AddEdge(parent[v], v, v_edge);
            MST.AddEdge(v, parent[v], v_edge);
        }

        for ( vertex_edge_t &ue: graph.GetNextVertices(v) )
        {
            size_t u      = ue.first;
            int    u_edge = ue.second;

            if ( min_e[u] > u_edge )
            {
                min_e[u]  = u_edge;
                parent[u] = v;
                queue.emplace(std::make_pair(u, u_edge));
            }
        }
    }
}

int MST_weight = 0;

int main() {
    size_t nvertices, nedges;
    size_t from, to;
    int    edge;

    std::cin >> nvertices >> nedges;

    CListGraph graph(nvertices);

    for ( auto i = 0; i < nedges; i++ )
    {
        std::cin >> from >> to >> edge;

        from--;
        to--;
        graph.AddEdge(from, to, edge);
        graph.AddEdge(to, from, edge);
    }

    CListGraph MST(nvertices);

    Prim(graph, MST, 0);

    MST_weight = 0;
    BFS(MST, 0, [](size_t vertex, int edge){ MST_weight += edge; });

    std::cout << MST_weight;
    return 0;
}