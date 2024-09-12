//  tour_generator.h
//  CS32Project4

#ifndef tour_generator_h
#define tour_generator_h

// do a header file check
#include "base_classes.h"
// do we not need these two??????
#include "geodb.h"
#include "router.h"




class TourGenerator: public TourGeneratorBase
{
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator();
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const GeoDatabaseBase* m_geodb;
    const RouterBase* m_router;
    string getDirection(double angle) const;
    string getTurnDirection(double angle) const;
    bool sameStreet(const GeoPoint &p1, const GeoPoint &p2, const GeoPoint &p3) const;
};

#endif /* tour_generator_h */
