#include "graph.h"

#include <vector>
#include <queue>
#include <iostream>
/// Return shortest path from start to end as array of vertices.
std::vector<Point> shortest_path(const Graph& graph, Point start_vertex, Point end_vertex, int unit);
std::vector<Point> build_path(std::map<Point, Point>& parent, Point start_vertex, Point end_vertex);