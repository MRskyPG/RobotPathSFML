#include "shortest_path.h"

using namespace std;
const auto INF = std::numeric_limits<int>::max();

vector<Point> build_path(map<Point, Point>& parent, Point start_vertex, Point end_vertex)
{
    vector<Point> result;
    Point current = end_vertex;
    result.push_back(current);
    if (current == start_vertex) return {};

    while (current != start_vertex)
    {

        current = parent[current];
        //Если предшествинника в кратч.пути нет (изначально стоит INF), то недостижима вершина из данной.
        if (current == Point(INF, INF) || current == end_vertex)
        {
            return {};
        }
        result.insert(result.begin(), current);
    }
    return result;
}


vector<Point> shortest_path(const Graph& graph, Point start_vertex, Point end_vertex, int unit)
{
    // Return shortest path in the graph from start vertex to end vertex as array of vertices.
   // First item in the result should be start vertex, last - end vertex.
   // Return empty array if there is no path.
    if (graph.get_vertices().empty()) return {};
    vector <Point> graph_vertices;
    priority_queue <pair<double, Point>> q;
    int len;
    Point u;
    double weight;


    graph_vertices = graph.get_vertices();
    //vector<double> distance(graph_vertices.size(), INF);
    map<Point, double> distance;
    map<Point, Point> parent;
    //Занести в parent большое число, которое в будущем будет заменяться на предшествинников в кратчайшем пути будет.
    //Если предшественника нет в ассоциативном массиве, значит из данной вершины тот не достижим.
    for (const auto& p : graph_vertices) {
        parent[p] = Point(INF, INF);
    }//O(n)

    for (const auto& p : graph_vertices) {
        distance[p] = INF;
    }

    distance[start_vertex] = 0;
    q.push(make_pair(0, start_vertex));

    while (!q.empty()) //O(1)
    {
        u = q.top().second;//O(1)

        q.pop(); //O(logn)

        if (u == end_vertex)
        {
            return build_path(parent, start_vertex, end_vertex);
        }

        //Для смежных вершин "v" с "u"
        for (auto& v : graph.get_adjacent_vertices(u)) //O(V)
        {
            weight = graph.edge_weight(u, v);
            bool hasUpLeftDiag = (v.x == u.x - unit && v.y == u.y - unit) &&
                (graph.has_edge(u, Point(u.x - unit, u.y)) && graph.has_edge(u, Point(u.x, u.y - unit)));

            bool hasUpRightDiag = (v.x == u.x + unit && v.y == u.y - unit) &&
                (graph.has_edge(u, Point(u.x + unit, u.y)) && graph.has_edge(u, Point(u.x, u.y - unit)));

            bool hasDownLeftDiag = (v.x == u.x - unit && v.y == u.y + unit) &&
                (graph.has_edge(u, Point(u.x - unit, u.y)) && graph.has_edge(u, Point(u.x, u.y + unit)));

            bool hasDownRightDiag = (v.x == u.x + unit && v.y == u.y + unit) &&
                ( graph.has_edge(u, Point(u.x + unit, u.y)) && graph.has_edge(u, Point(u.x, u.y + unit)));

            if (weight == 1.0 || (weight == 1.4 && (hasUpLeftDiag || hasUpRightDiag || hasDownLeftDiag || hasDownRightDiag))) {
                if (distance[v] > distance[u] + weight)
                {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;
                    q.push(make_pair(-distance[v], v));
                    //O(logV)
                }
            }
        
        }

    }
    return build_path(parent, start_vertex, end_vertex);

}
//O(n+2logn+VlogV) -> O(n + VlogV)