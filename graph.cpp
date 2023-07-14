#include "graph.h"
#include <stdexcept>

using namespace std;

Graph::Graph(initializer_list<tuple<Point, Point, double>> edges) {
    for (const auto& e : edges) {
        add_edge(get<0>(e), get<1>(e), get<2>(e));
    }
}

void Graph::add_vertex(Point vertex) {
    if (!has_vertex(vertex)) {
        vertices[vertex] = std::map<Point, double>();
    }
}

void Graph::add_edge(Point start_vertex, Point end_vertex, double weight) {
    add_vertex(start_vertex);
    add_vertex(end_vertex);
    if (!has_edge(start_vertex, end_vertex) || edge_weight(start_vertex, end_vertex) != weight) {
        vertices[start_vertex][end_vertex] = weight;
        vertices[end_vertex][start_vertex] = weight;
    }
}

std::vector<Point> Graph::get_vertices() const {
    std::vector<Point> result;
    for (const auto& p : vertices) {
        result.push_back(p.first);
    }
    return result;
}

std::vector<Point> Graph::get_adjacent_vertices(Point src_vertex) const {
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return std::vector<Point> {};
    }
    vector<Point> result;
    for (const auto& p : it->second) {
        result.push_back(p.first);
    }
    return result;
}

vector<pair<Point, double>> Graph::get_adjacent_edges(Point src_vertex) const {
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return vector<pair<Point, double>> {};
    }
    vector<pair<Point, double>> result;
    for (const auto& p : it->second) {
        result.push_back(make_pair(p.first, p.second));
    }
    return result;
}

bool Graph::has_vertex(Point vertex) const {
    return (vertices.find(vertex) != vertices.end());
}

bool Graph::has_edge(Point start_vertex, Point end_vertex) const {
    const auto it = vertices.find(start_vertex);
    if (it == vertices.end()) {
        return false;
    }
    return (it->second.find(end_vertex) != it->second.end());
}

double Graph::edge_weight(Point start_vertex, Point end_vertex) const {
    const auto it_s = vertices.find(start_vertex);
    if (it_s == vertices.end()) {
        throw invalid_argument("Edge doesn't exist");
    }
    const auto it_e = it_s->second.find(end_vertex);
    if (it_e == it_s->second.end()) {
        throw invalid_argument("Edge doesn't exist");
    }
    return it_e->second;
}

void Graph::remove_vertex(Point vertex) {
    /// Remove adjacent edges.
    auto adjacent_vertices = get_adjacent_vertices(vertex);
    for (const auto& adj_v : adjacent_vertices) {
        remove_edge(adj_v, vertex);
    }
    /// Remove the vertex itself.
    vertices.erase(vertex);
}

void Graph::remove_edge(Point start_vertex, Point end_vertex) {
    auto it_s = vertices.find(start_vertex);
    if (it_s != vertices.end()) {
        it_s->second.erase(end_vertex);
    }
    auto it_e = vertices.find(end_vertex);
    if (it_e != vertices.end()) {
        it_e->second.erase(start_vertex);
    }
}