#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <initializer_list>
#include <SFML/Graphics.hpp>
#include "objects.h"

using namespace robopath;
/**
 * Non-oriented weighted graph.
**/
class Graph {
public:
    Graph() {}
    Graph(std::initializer_list<std::tuple<Point, Point, double>> edges);
    /// Add single vertex to the graph.
    void add_vertex(Point vertex);
   
    /// Add vertices (if necessary) and an edge between them to the graph.
    void add_edge(Point start_vertex, Point end_vertex, double weight = 1.0);

    /// Return all vertices of the graph.
    std::vector<Point> get_vertices() const;

    /// Return all adjacent by edges vertices for the specified vertex.
    std::vector<Point> get_adjacent_vertices(Point src_vertex) const;

    /// Get adjacent edges for the vertex as vector of (end vertex, edge weight).
    std::vector<std::pair<Point, double>> get_adjacent_edges(Point src_vertex) const;

    /// Return true if the vertex exists in the graph, false otherwise.
    bool has_vertex(Point vertex) const;

    /// Return true if vertices exist and have an edge between them, false otherwise.
    bool has_edge(Point start_vertex, Point end_vertex) const;

    /// Return weight of an edge between vertices (if there is any), throw an exception otherwise.
    double edge_weight(Point start_vertex, Point end_vertex) const;

    /// Remove the vertex and all its adjacent edges from the graph.
    void remove_vertex(Point vertex);

    /// Remove the edge from the graph (but not the vertices).
    void remove_edge(Point start_vertex, Point end_vertex);

private:
    std::map<Point, std::map<Point, double>> vertices;
    
};