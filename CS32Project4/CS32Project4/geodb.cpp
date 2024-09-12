//  geodb.cpp
//  CS32Project4

#include "geodb.h"
#include "geotools.h"
#include "base_classes.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert> // TO BE DELETED!!!!
#include <string>

using namespace std;

// load all the data from an open maps data file (mapdata.txt)
// organize it in HashMap
bool GeoDatabase::load(const string& map_data_file) {
    
    ifstream infile(map_data_file);    // infile is a name of our choosing
    if (! infile )                // Did opening the file fail?
    {
        cerr << "Error: Cannot open " << map_data_file << "!" << endl;
        return false;
    }
    
    int num_pois = 0; // number of points of interest
    
    string line;
    while (getline(infile, line))
    {
        // first line - street name
        istringstream iss(line);
        string streetname;
        streetname = line;

        // second line - starting and ending geopoints
        getline(infile, line);
        string geopoints = line;
        istringstream iss2(line);

        string lat1, lon1, lat2, lon2; // parse through latitude and longitude
        iss2 >> lat1 >> lon1 >> lat2 >> lon2;
        
        // construct geopoints for start and end loc
        GeoPoint point1(lat1, lon1);
        GeoPoint point2(lat2, lon2);
        
        string s1 = point1.to_string();
        string s2 = point2.to_string();
        
        // if a matching geopoint in database, then add to the vector of geopoints
        // s1 <--> s2
        updateConnected(s1, point2);
        updateConnected(s2, point1);
        updateStreetNames(s1, s2, streetname);
        
        GeoPoint mid_point = midpoint(point1, point2); // calculates the midpoint
        string m = mid_point.to_string();
        
        updateStreetNames(s1, m, streetname);
        updateStreetNames(s2, m, streetname);
        
        // third line - holds a num that specifies P points of interest
        if (getline(infile, line)) {
            
            num_pois = stoi(line); // string to integer conversion
           
            if (num_pois != 0) {
                // if a matching geopoint in database, then add to the vector of geopoints
                // s1 <--> midpoint
                updateConnected(s1, mid_point);
                updateConnected(m, point1);
                
                // s2 <--> midpoint
                updateConnected(s2, mid_point);
                updateConnected(m, point2);
 
                for (int i = 0; i < num_pois; i++) {
                    if (getline(infile, line)) {
                        string name, lat, lon, poi_gp;
                        
                        istringstream iss(line);
                        getline(iss, name, '|');
                        iss >> lat >> lon;
                        
                        GeoPoint poi_geopoint(lat, lon); // create poi geopoint
                        poi_database.insert(name, poi_geopoint); // add to poi map
                        poi_gp = poi_geopoint.to_string();
                    
                        // midpoint <--> poi
                        updateConnected(m, poi_geopoint);
                        updateConnected(poi_gp, mid_point);
                        
                        updateStreetNames(poi_gp, m, "a path");
                    }
                }
            }
        }
    }
    cerr << "Success: File was opened" << endl;
    return true;
}

// perhaps make a hashmap that maps string names of poi --> GeoPoints, then just return the point
bool GeoDatabase::get_poi_location(const string& poi, GeoPoint& point) const {
    if (poi_database.find(poi) != nullptr) {
        point = *poi_database.find(poi);
        return true;
    }
    return false;
}

// takes in one geopoint turns into a string, then maps to a vector of connected points
std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    vector<GeoPoint> gp; // geopoint vector
    string s_pt = pt.to_string();

    if (connected_database.find(s_pt) != nullptr)
        gp = *connected_database.find(s_pt);
    
    return gp;
}

// perhaps make a hashmap that has the whole geopoint string --> that then maps to strings names
std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
    
    // 1) if two endpoints are ends of street segment --> return street name
    // 2) if two endpoints connect poi with midpoint/midpoint with poi --> set name to "a path"
    // 3) if two endpoints connect a midpoint with the end of a street segment (and vice versa) --> return street name with overall segment
    
    string name = "";
    string pts = pt1.to_string() + "," + pt2.to_string();
    if (street_database.find(pts) != nullptr) 
        name = *street_database.find(pts);

    //street_database.print();
    return name;
}

void GeoDatabase::updateConnected(const string& name, const GeoPoint& point) {
    if (connected_database.find(name) != nullptr) {
        connected_database.find(name)->push_back(point);
    }
    else {
        vector<GeoPoint> to_insert;
        to_insert.push_back(point);
        connected_database.insert(name, to_insert);
    }
}

void GeoDatabase::updateStreetNames(const string& s1, const string& s2, const string& streetname) {
    // bidirectional
    string streetloc1 = s1 + "," + s2;
    string streetloc2 = s2 + "," + s1;
    street_database.insert(streetloc1, streetname);
    street_database.insert(streetloc2, streetname);
}

// TO BE DELETED !!!! FOR TESTING PURPOSES
//int main() {
//    GeoDatabase g;
//    g.load("/Users/katiechung/Desktop/UCLA CS 32/CS32Project4/CS32Project4/mapdata.txt");
//    
//    GeoPoint p;
//     if (g.get_poi_location("Diddy Riese", p))
//         assert(p.sLatitude == "34.0630614" && p.sLongitude == "-118.4468781");
//
//    if (g.get_poi_location("Mr. Noodle", p))
//        assert(p.sLatitude == "34.0629463" && p.sLongitude == "-118.4468728");
//   
//    if (g.get_poi_location("Fox Theater", p))
//        assert(p.sLatitude == "34.0626647" && p.sLongitude == "-118.4472813");
//
//   std::vector<GeoPoint> pts = g.get_connected_points(GeoPoint("34.0736122", "-118.4927669"));
//     if (pts.empty())
//         cout << "There are no points connected to your specified point\n";
//     else {
//         for (const auto p: pts)
//            cout << p.sLatitude << ", " << p.sLongitude << endl;
//         cout << "it actually worked";
//    }
//    
//    GeoPoint p1("34.0732851", "-118.4931016");
//    GeoPoint p2("34.0736122", "-118.4927669");
//    string one;
//    one = g.get_street_name(p1, p2); // writes "Glenmere Way"
//    string two;
//    two = g.get_street_name(p2, p1); // writes "Glenmere Way"
//    
//    GeoPoint p3("34.0601422", "-118.4468929");
//    GeoPoint p4("34.0600768", "-118.4467216");
//    string three;
//    three = g.get_street_name(p3, p4); // writes "a path"
//    string four;
//    four = g.get_street_name(p4, p3); // writes "a path"
//    
//    GeoPoint p5("34.0602175", "-118.4464952");
//    GeoPoint p6("34.0600768", "-118.4467216");
//    string five;
//    five = g.get_street_name(p5, p6); // writes "Kinross Avenue"
//    string six;
//    six = g.get_street_name(p6, p5); // writes "Kinross Avenue"
//    
//    assert(one == "Glenmere Way");
//    assert(two == "Glenmere Way");
//    assert(three == "a path");
//    assert(four == "a path");
//    assert(five == "Kinross Avenue");
//    assert(six == "Kinross Avenue");
//
//    cerr << "Passed this";
//}
//
//
