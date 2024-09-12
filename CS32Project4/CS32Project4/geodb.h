//  geodb.hpp
//  CS32Project4

#ifndef geodb_h
#define geodb_h

#include "base_classes.h"
#include "hashmap.h"

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase() {}
    virtual ~GeoDatabase() {}

    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    HashMap<GeoPoint> poi_database;
    HashMap<vector<GeoPoint>> connected_database;
    HashMap<string> street_database;
    
    // helper functions
    void updateConnected(const string& name, const GeoPoint& point);
    void updateStreetNames(const string& s1, const string& s2, const string& streetname);
};

#endif /* geodb_h */
