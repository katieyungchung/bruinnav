//  tour_generator.cpp
//  CS32Project4

#include <iostream>
#include "tour_generator.h"
#include "geotools.h"
using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) {
    m_geodb = &geodb;
    m_router = &router;
}

TourGenerator::~TourGenerator() {}
std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
    
    vector<TourCommand> result; // holds the instructions
    int j = 0;
    while (j != stops.size()) {
        string m_poi1, m_comm1 = "";
        stops.get_poi_data(j, m_poi1, m_comm1);
        
        TourCommand tc1;
        tc1.init_commentary(m_poi1, m_comm1); // commentary
        result.push_back(tc1); // add tourcommand to result vector
        
        // if there's only one stop, end tour
        if (stops.size() == 1)
            break;
    
        // if there is another poi
        
        string m_poi2, m_comm2 = "";
        stops.get_poi_data(j + 1, m_poi2, m_comm2);
        
        GeoPoint p1, p2; // initialize geopoints
        m_geodb->get_poi_location(m_poi1, p1); // update value
        m_geodb->get_poi_location(m_poi2, p2); // update value
        
        vector<GeoPoint> path = m_router->route(p1, p2);
        for (auto p : path) {
            cout << p.to_string() << endl;
        }
        
        int i = 0;
        while (i + 1 < path.size()) {
            // proceed and turn instructions
            
            // initialize GeoPoints
            GeoPoint g1 = path[i];
            GeoPoint g2 = path[i + 1];
            
            // find the distance and street name
            double dis = distance_earth_miles(g1, g2);
            string street = m_geodb->get_street_name(g1, g2);
            
            // get the angle between two GeoPoints
            double angle = angle_of_line(g1, g2);
            string dir = getDirection(angle);
            
            // proceed --> every two adjacent GeoPoints p1 and p2
            TourCommand tc2;
            tc2.init_proceed(dir, street, dis, g1, g2);
//            cerr << "dis: " << dis << endl;
//            cerr << "street: " << street << endl;
//            cerr << "angle: " << angle << endl;
//            cerr << "dir: " << dir << endl;
            result.push_back(tc2);
            
            
            // if there is a third point
            if (i + 2 < path.size()) {
                
                TourCommand tc3;
                GeoPoint p3 = path[i + 2]; // initalize 3rd point
                double ang = angle_of_turn(p1, p2, p3); // calculate the angle
            
                
                if (!sameStreet(p1, p2, p3) && ang == 0) // if on the same street or no turn
                    break;
         
                string turn = getTurnDirection(ang);
                string str_name = m_geodb->get_street_name(p2, p3);
                tc3.init_turn(turn, str_name);
                result.push_back(tc3);
                i++;
                continue;
            }
            
            i++;
        }
        j++;
    }
    return result;
}


string TourGenerator::getDirection(double angle) const {
    string dir = "";
    if (angle < 0) {
        cerr << "Invalid angle";
        return dir;
    }
    else if (angle < 22.5) { dir = "east"; }
    else if (angle < 67.5) { dir = "northeast"; }
    else if (angle < 112.5) { dir = "north"; }
    else if (angle < 157.5) { dir = "northwest"; }
    else if (angle < 202.5) { dir = "west"; }
    else if (angle < 247.5) { dir = "southwest"; }
    else if (angle < 292.5) { dir = "south"; }
    else if (angle < 337.5) { dir = "southeast"; }
    else { dir = "east"; }
    return dir;
}

string TourGenerator::getTurnDirection(double angle) const {
    string dir = "";
    if (angle >= 1 && angle < 180) {
        dir = "left";
    }
    else if (angle >= 180 && angle <= 359) {
        dir = "right";
    }
    else {
        cerr << "Invalid angle";
    }
    return dir;
}

bool TourGenerator::sameStreet(const GeoPoint &p1, const GeoPoint &p2, const GeoPoint &p3) const {
    // the street name of first segment (p1->p2) and street name of second
    // segment p2->p3 differ
    return (m_geodb->get_street_name(p1, p2) != m_geodb->get_street_name(p2, p3));
}
//
//int main() {
//    cerr << "testinggg" << endl;
//    
//    GeoDatabaseBase* geodb = new GeoDatabase();
//    geodb->load("/Users/katiechung/Desktop/UCLA CS 32/CS32Project4/CS32Project4/mapdata.txt");
//    RouterBase* router = new Router(*geodb);
//    TourGeneratorBase* tgb = new TourGenerator(*geodb, *router);
//    Stops stop;
//    stop.load("/Users/katiechung/Desktop/UCLA CS 32/CS32Project4/CS32Project4/stops.txt");
//    
//    vector<TourCommand> tc = tgb->generate_tour(stop);
//    for (auto t : tc) {
//        cout << "poi: " << t.get_poi() << endl;
//        cout << "street:" << t.get_street() << endl;
//        cout << "distance: " << t.get_distance() << endl;
//        cout << "direction: " << t.get_direction() << endl;
//        cout << "commentary: " << t.get_commentary()<< endl;
//    }
//   
//    cerr << "Passed tests!" << endl;
//}
