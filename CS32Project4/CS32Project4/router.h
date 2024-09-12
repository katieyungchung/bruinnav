//  router.h
//  CS32Project4


#ifndef router_h
#define router_h

#include "geodb.h"
#include "geopoint.h"
#include <vector>
#include <queue>
#include "hashmap.h"
using namespace std;

/*
 The Router class is responsible for computing an efficient route from a source GeoPoint s to a
 destination GeoPoint d, if one exists (a "route" is a vector of GeoPoints that are directly
 connected to each other, including points s and d). It must use the GeoDatabase class to stitch
 together a set of waypoints (GeoPoints) between the starting and ending GeoPoints.
 */

class Router: public RouterBase
{
public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    struct Node {
        GeoPoint point;
//        double g_cost; // distance from starting point
//        double h_cost; // distance from ending point
        double f_cost; // sum of g and h cost
        // Node* parent;
        
        bool operator>(const Node& other) const {
            return f_cost > other.f_cost;
        }
        bool operator<=(const Node& other) const {
            return f_cost <= other.f_cost;
        }
        bool operator==(const Node& other) const {
            return point.to_string() == other.point.to_string();
        }
    };
    
    const GeoDatabaseBase* gdb; // geodatabasebase pointer
    vector<GeoPoint> generatePath();
    mutable HashMap<GeoPoint> visited; // keep track if a geopoint has been visited
    void explore(queue<GeoPoint>& toDo, const GeoPoint start_pt, const GeoPoint end_pt) const;

};


#endif /* router_h */
