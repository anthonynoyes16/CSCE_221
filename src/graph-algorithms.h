#pragma once

#include <istream>
#include <ostream>
#include <sstream>
#include <limits>
#include <list>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "weighted-graph.hpp"

#include "graph-types.h"

// if the arrow is a box, change to the other line
#define ARROW_SEPARATOR " \u2192 "
// #define ARROW_SEPARATOR " -> "

// contains initializeSingleSource, relax, & updateHeap as well as the DijkstraComaparator
#include "dijkstras-helpers.h"

/**
 * @brief Dijkstra's Algorithm - https://canvas.tamu.edu/courses/136654/files/35930572/preview Slide 16
 *
 * @tparam T type of data stored by a vertex
 * @param graph weighted, directed graph to find single-source shortest-path
 * @param initial_node source node in graph for shortest path
 * @param destination_node destination node in graph for shortest path
 * @return std::list<value_type<T>> list of nodes along shortest path including initial_node and destination_node, empty if no path exists
 */
template <typename T>
std::list<value_type<T>> dijkstrasAlgorithm(const WeightedGraph<T>& graph, vertex_type<T> initial_node, vertex_type<T> destination_node)
{
    std::unordered_map<value_type<T>, weight_type<T>> distances;
    std::unordered_map<value_type<T>, std::optional<value_type<T>>> predecessors;
    // TODO implement Dijkstra's Algorithm

    // TODO create list by walking backwards through predecessors from the end

    // initializeSingleSource(graph, initial_node)
    initializeSingleSource(graph, initial_node, distances, predecessors);

    // s = [] # Set
    // q = [] # Priority Queue
    // q.extend([v for v in graph])
    std::unordered_set<value_type<T>> s;
    std::priority_queue<value_type<T>, std::vector<value_type<T>>, DijkstraComparator<T>> q(DijkstraComparator<T>{distances});
    for (const auto & ele1 : graph) {
            q.push(ele1.first);
    }

    // while len(q) > 0:
    while (q.size() > 0) {
    //     u = min(q)
    //     if distance[u] == infinity():
    //         break
    //     q.remove(u)
        value_type<T> u = q.top();
        if (distances.at(u) == std::numeric_limits<weight_type<T>>::max()) {
            break;
        }
        q.pop();

    //     if u not in s:
    //         s.append(u)
        if (s.find(u) == std::unordered_set<unsigned int>::iterator(nullptr)) {
            s.insert(u);
        }

    //     for pair in u.adj_list:
        adjacency_list<T> adj_list = graph.at(u);
        for (auto it : adj_list) {
    //         v = pair.first
    //         if v in s:
    //             continue
    //         w = pair.second
    //         r = relax(u, v, w)
    //         if r:
    //             updateHeap(q)
            value_type<T> v = it.first;
            if (s.find(v) != std::unordered_set<unsigned int>::iterator(nullptr)) {
                continue;
            }
            weight_type<T> w = it.second;
            bool r = relax<value_type<T>>(u, v, w, distances, predecessors);
            if (r) {
                updateHeap(q, distances);
            }
        }
    }
    
    // l = [] # List
    std::list<value_type<T>> lst;

    // node = destination_node
    value_type<T> node = destination_node;

    // while predecessor[node] is not None:
    while (predecessors[node].has_value()) {
    //     l = [node] + l # prepend node to l
    //     node = predecessor[node]
        lst.push_front(node);
        node = predecessors.at(node).value();
    }
    

    // if len(l) > 0 or initial_node == destination_node:
    //     l = [initial_node] + l
    if (lst.size() > 0 or initial_node == destination_node) {
        lst.push_front(node);
    }
    // return l
    return lst;
}

#include "top-sort-helpers.h"

/**
 * @brief Returns a Topological Ordering of the Graph - https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search
 *
 * @tparam T type of data stored by a vertex
 * @param graph graph upon which to perform a topological ordering
 * @return std::list<value_type<T>> list of nodes in a topological order, or an empty list if no such ordering exists
 */
template <typename T>
std::list<value_type<T>> topologicalSort(const WeightedGraph<T>& graph)
{
    std::unordered_map<value_type<T>, int> indegrees;
    std::unordered_map<value_type<T>, int> topological_numbers;
    computeIndegrees(graph,indegrees);

    // q = [] # Queue
    std::queue<value_type<T>> Q;
    // l = [] # List
    std::list<value_type<T>> lst;

    // q.extend([v for v in graph if indegree[v] == 0])
    for (const auto & ele1 : indegrees) {
        if (ele1.second == 0) {
            Q.push(ele1.first);
        }
    }

    // while len(q) > 0:
    //     v = q.pop(0) # remove first
    //     l.append(v)
    while (!Q.empty()) {
        value_type<T> hold = Q.front();
        lst.push_back(hold);
        Q.pop();
        
        //     for adj_v in v.adj_list:
        //         indegree[adj_v] -= 1
        //         if indegree[adj_v] == 0:
        //             q.push(adj_v)
        for (auto element1 = graph.at(hold).begin(); element1 != graph.at(hold).end(); element1++){
            indegrees[element1->first]--;
            if (indegrees[element1->first] == 0) {
                Q.push(element1->first);
            }
        }
    }

    // if len(l) != len(graph):
    //     l.clear()
    if (lst.size() != graph.size()) {
        lst.clear();
    }

    // return l
    return lst;
}

template <typename T>
std::ostream &operator<<(std::ostream &o, const WeightedGraph<T> &graph)
{
    for (auto it = graph.begin(); it != graph.end(); ++it)
    {
        const vertex_type<T> &vertex = it->first;
        const adjacency_list<T> &list = it->second;
        o << vertex << ": ";
        for (auto iit = list.begin(); iit != list.end(); ++iit)
        {
            const vertex_type<T> &destination = iit->first;
            const weight_type<T> &weight = iit->second;
            o << destination << "(" << weight << ')';
            auto next_iter = iit;
            if (++next_iter != list.end())
            {
                o << ARROW_SEPARATOR;
            }
        }
        auto next_iter = it;
        if (++next_iter != graph.end())
        {
            o << "\n";
        }
    }
    return o;
}

template <typename T>
std::istream &readEdge(std::istream &i, value_type<T> &vertex, weight_type<T> &weight)
{
    std::string s_vertex, s_weight;
    std::getline(i, s_vertex, '(');
    std::getline(i, s_weight, ')');
    std::istringstream stream(s_vertex + " " + s_weight);
    stream >> vertex >> weight;
    if (stream.fail())
    {
        i.setf(stream.flags());
    }
    return i;
}

template <typename T>
std::istream &operator>>(std::istream &i, WeightedGraph<T> &graph)
{
    std::string line;
    while (std::getline(i, line))
    {
        if (line.empty())
            break;
        std::istringstream line_stream(line);
        value_type<T> vertex;
        std::string s_vertex;
        std::getline(line_stream, s_vertex, ':');
        std::istringstream stream_vertex(s_vertex);
        stream_vertex >> vertex;
        if (stream_vertex.fail())
        {
            i.setf(stream_vertex.flags());
            break;
        }
        graph.push_vertex(vertex);
        std::string separator;
        value_type<T> edge_end;
        weight_type<T> edge_weight;
        while (readEdge<T>(line_stream, edge_end, edge_weight))
        {
            line_stream >> separator;
            graph.push_edge(vertex, edge_end, edge_weight);
        }
    }

    if(i.eof() and i.fail())
        i.clear(std::ios::eofbit);
    
    return i;
}
