//  router.cpp
//  CS32Project4

#include "router.h"
#include <iostream>
#include <queue>
#include <cassert> // to delete????
#include "geotools.h"
#include <algorithm>
using namespace std;

// constructor
Router::Router(const GeoDatabaseBase& geo_db) : gdb(&geo_db) {}

// destructor
Router::~Router() {}

// route function

/*
 
 g = distance from starting
 h = distance from ending
 f = g + h
 
 */
//
std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    // priority queue // item to store, internal container used to store, greater is a custom comparison function
    priority_queue<Node, vector<Node>, greater<Node>> openList;
    vector<Node> open; // can't traverse through priority queue
    vector<Node> closedList;
    HashMap<GeoPoint> visited; // keeps track of a geopoint, and what geopoint they came from
    HashMap<double> gCost;
    vector<GeoPoint> path; // path to be returned, constructed from visited
    
    // first node is the starting point
    Node start; // lowkey make a constructor for this !!!!!!!!!!!!
    start.point = pt1;
    start.f_cost = distance_earth_miles(pt1, pt2);
    openList.push(start);
    open.push_back(start);
    gCost.insert(pt1.to_string(), 0); // starting point has g cost of 0
    
    while (!openList.empty()) {
        Node curr = openList.top(); // node w/ lowest f_cost
        openList.pop(); // remove from openList
        // remove from open vector
        auto it = find(open.begin(), open.end(), curr);
        if (it != open.end())
            open.erase(it);
        closedList.push_back(curr);
        
      //  cerr << "curr point" << curr.point.to_string() << endl;
        
        if (curr.point.to_string() == pt2.to_string()) // made it to the end point
        {
            GeoPoint g = pt2;
            path.push_back(g);
            
//            cerr << "HEY !!!" << (visited.find(g.to_string())->to_string()) << endl;
//            string h = visited.find(g.to_string())->to_string();
//            cerr << "HEY ~!!" << visited.find(h)->to_string() << endl;
//            
//            
//            string s = visited.find(h)->to_string();
//            cerr << "HEY ~!!!" << visited.find(s)->to_string() << endl;
//            visited.print();
            
            
            while (visited.find(g.to_string()) != nullptr) {
                if (g.to_string() == pt1.to_string()) {
                    reverse(path.begin(), path.end());
                    return path;
                }
                else {
                    GeoPoint* h = visited.find(g.to_string());
                    path.push_back(*h);
                    g = *h;
                }
            }
            return path;
//            cerr << "happening we in here";
//            GeoPoint g = pt2;
//            path.push_back(g);
//            while (g.to_string() != pt1.to_string()) {
//                cerr << "entered loop";
//                if (visited.find(g.to_string()) == nullptr) {
//                    cerr << "no path???";
//                    return path;
//                }
//                else {
//                    GeoPoint* h = visited.find(g.to_string());
//                    path.push_back(*h);
//                    g = *h;
//                }
//            }
//            reverse(path.begin(), path.end());
//            return path;
        }
        
        // get the vector of geopoints connected to the current point
        vector<GeoPoint> connected = gdb->get_connected_points(curr.point);
   
        for (auto c: connected) { // for each geopoint
            Node newNode;
            newNode.point = c;

            if (find(closedList.begin(), closedList.end(), newNode) != closedList.end())
                continue;
            
            double m_g = 0;
            if (gCost.find(c.to_string()) != nullptr)
                m_g = *gCost.find(c.to_string());
            double g_cost = m_g + distance_earth_miles(curr.point, c); // distance from start
            double h_cost = distance_earth_miles(c, pt2); // distance from end
            newNode.f_cost = g_cost + h_cost;
            
            if (find(open.begin(), open.end(), newNode) == open.end()) {
                open.push_back(newNode);
                openList.push(newNode);
            }
            else if (find(open.begin(), open.end(), newNode) != open.end()) {
                double* other_g = gCost.find(newNode.point.to_string());
                if (g_cost >= *other_g) {
                    continue;
                }
            }
            gCost.insert(newNode.point.to_string(), g_cost);
            visited.insert(newNode.point.to_string(), curr.point);
            
            
            
//            Node newNode;
//            newNode.point = c;
//            
//            double m_g = 0;
//            if (gCost.find(c.to_string()) != nullptr)
//                m_g = *gCost.find(c.to_string());
//                
//            double g_cost = m_g + distance_earth_miles(curr.point, c); // distance from start
//            
//            double h_cost = distance_earth_miles(c, pt2); // distance from end
//            newNode.f_cost = g_cost + h_cost;
//            
//            cerr << "got here";
//            
//            // if node w/ the same position as successor is in open list and has a lower f
//            // skip this successor
//            auto it1 = find(open.begin(), open.end(), newNode);
//            if (it1 != open.end() && it1->f_cost < curr.f_cost)
//                continue;
//            
//            // if a node w/ the same position as successor is in closed list and has lower f
//            // skip this successor
//            auto it2 = find(closedList.begin(), closedList.end(), newNode);
//            if (it2 != open.end() && it2->f_cost < curr.f_cost)
//                continue;
//            
//            // add to openlist
//            openList.push(newNode);
//           //  cerr << "top of list" << openList.top().point.to_string();
//            open.push_back(newNode);
//            
//            
//            if (c.to_string() != pt1.to_string()) {
//                visited.insert(c.to_string(), curr.point);
//                cerr << endl;
//                cerr << "visited: " << c.to_string() << endl;
//                cerr << "visited before: " << curr.point.to_string() << endl;
//                cerr << endl;
//            }
//            
//            gCost.insert(c.to_string(), g_cost);
        }
    }
    return path;
}

//
//std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
//    queue<GeoPoint> toDo; 
//    vector<GeoPoint> path;
// 
//    // explore(toDo, pt1);
//    toDo.push(pt1);
//    
//    while (! toDo.empty() ) {
//        GeoPoint curr = toDo.front();
//        toDo.pop();
//        
//        if (curr.to_string() == pt2.to_string()) { // if made it to the end point, return the path
//            GeoPoint g = pt2;
//            path.push_back(g);
//            while (g.to_string() != pt1.to_string()) {
//                if (visited.find(g.to_string()) == nullptr) {
//                    return path;
//                }
//                else {
//                    const GeoPoint* h = visited.find(g.to_string());
//                    path.push_back(*h);
//                    g = *h;
//                }
//            }
//            
//            reverse(path.begin(), path.end());
//            return path;
//        }
//        
//        vector<GeoPoint> v = gdb->get_connected_points(curr);
//        for (int i = 0; i < v.size(); i++) {
//            explore(toDo, curr, v[i]); // explore each index
//        }
//    }
//    return path;
//}
//
//void Router::explore(queue<GeoPoint>& toDo, const GeoPoint start_pt, const GeoPoint end_pt) const {
//    string end = end_pt.to_string();
//    if (visited.find(end) == nullptr) { // if geopoint has not been visited yet
//        toDo.push(end_pt); // push it onto queue of geopoints to check
//        visited.insert(end_pt.to_string(), start_pt);
//    }
//}



// keep track of current path in a vector
// you can also keep track by using a hashmap - which helps with setting up the vector
// then you also you need to keep track of already visited geopoints
// use geodatabase class to identify adjacent coordinates
// get connected points function
// start with a queue of geopoints
// explore the starting point --> if

/*
 
 const GeoDatabaseBase* gdb; // geodatabasebase pointer
 
 queue<GeoPoint> toDo; // queue of geopoints that need to be visited
 HashMap<bool> visited; // keep track if a geopoint has been visited
 
 HashMap<GeoPoint> trackingPath; // helper hashmap to construct the path vector
 vector<GeoPoint> path; // path to return
 
 */

/*
 
 OPEN // set of nodes to be evaluated
 CLOSED // set of nodes already evaluated
 add the start node to OPEN
 
 loop
    current = node in OPEN with the lowest f_cost
    remove current from OPEN
    add current to CLOSED
 
    if current is the target node
        return
    for each neighbor of the current node
        if neighbor is not traversable or neighbor is CLOSED
            skip to next neighbor
        if new path to neighbor is shorter OR neighbor is not in open
            set f_cost of neighbor
            set parent of nieghbor to current
            if neighbor is not in OPEN
                add nieghbor to OPEN
 
 
 Initialization:
    Start by initializing an open set (priority queue) to store nodes to be evaluated.
    Initialize a closed set (unordered map) to store visited nodes.
    Add the start node to the open set.
 
 Main Loop:
     While the open set is not empty:
     Pop the node with the lowest total cost (f score) from the open set.
     If the current node is the goal node, reconstruct the path and return it.
     Otherwise, mark the current node as visited by adding it to the closed set.
     Generate the neighboring nodes of the current node.
     For each neighbor:
     If it's already in the closed set, skip it.
     Calculate the tentative g score (cost from start to neighbor through the current node).
     If the neighbor is not in the open set or the tentative g score is lower than its previous score:
     Update the neighbor's g score.
     Calculate the heuristic (h score) for the neighbor (estimated cost from neighbor to goal).
     Update the neighbor's parent to the current node.
     Add the neighbor to the open set.
 
 Path Reconstruction:
     If the open set becomes empty without finding the goal node, return an empty path.
     Reconstruct the path from the goal node by following the parent pointers from the goal node to the start node.
     Reverse the path to get it in the correct order.
     Return the path.
 
 
 
 */
//
//int main() {
//    GeoDatabase g;
//    g.load("/Users/katiechung/Desktop/UCLA CS 32/CS32Project4/CS32Project4/mapdata.txt");
//
//    GeoDatabaseBase* gdb = &g;
//    Router r(*gdb);
//    
//    GeoPoint p1("0", "0");
//    GeoPoint p2("3", "3");
//    vector<GeoPoint> test = r.route(p1, p2);
//    
//    if (test.empty())
//        cerr << "no path found" << endl;
//    cout << endl;
//    for (int i = 0; i < test.size(); i++) {
//        cerr << test[i].to_string() << endl;
//    }
//    cerr << "Path found!" << endl;
//    
//    cerr << "Passed all tests" << endl;
//}
