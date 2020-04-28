#include "provided.h"
#include <list>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <iostream>
#include "ExpandableHashMap.h"
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    struct LowestFScore {
    public:
        LowestFScore(GeoCoord a, double v) : m_coord(a), m_fScore(v) {}
        GeoCoord m_coord;
        double m_fScore;    
    };    
    struct CompareFScore {
        bool operator()(LowestFScore const& p1, LowestFScore const& p2)
        {
            // return "true" if "p1" is ordered  
            // before "p2", for example: 
            return p1.m_fScore > p2.m_fScore;
        }
    };
    const StreetMap* m_sm;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_sm = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
    const GeoCoord& start,
    const GeoCoord& end,
    list<StreetSegment>& route,
    double& totalDistanceTravelled) const
{
    //Reset route and dist in case
    route.clear();
    totalDistanceTravelled = 0;
    cerr << "Called Routes" << endl;
    //Bad Ending or Starting Coordinates
    vector<StreetSegment> segs;
    if (!(m_sm->getSegmentsThatStartWith(start, segs)) || !(m_sm->getSegmentsThatStartWith(end, segs))) {
        cerr << "Bad Coordinates" << endl;
        return BAD_COORD;  // invalid start or end
    }
    //openSet
    priority_queue<LowestFScore, vector<LowestFScore>, CompareFScore> openSet;
    openSet.push(LowestFScore(start, 0));
    //cameFrom
    ExpandableHashMap<GeoCoord, GeoCoord> came_from;
    //gScore
    ExpandableHashMap<GeoCoord, double> gScore;
    gScore.associate(start, 0);
    //fScore
    ExpandableHashMap<GeoCoord, double> fScore;
    fScore.associate(start, distanceEarthMiles(start, end));

    //A* Algorithm, prioritizes the lowest distance first
    while (!openSet.empty()) {
        GeoCoord current = openSet.top().m_coord;
        if (current == end) { //Found path to the end
            while (came_from.find(current) != nullptr) {
                GeoCoord* temp = came_from.find(current);
                if (m_sm->getSegmentsThatStartWith(current, segs)) { //Should always return true
                    for (auto it = segs.begin(); it != segs.end(); it++) {
                        if (it->end == *temp) { //Finds segment with to point
                            route.push_front(*it);
                            totalDistanceTravelled = distanceEarthMiles(it->start, it->end);
                            break;
                        }
                    }
                }
                current = *temp;
            }
            cerr << "Delivery" << endl;
            return DELIVERY_SUCCESS;
        }
        openSet.pop();
        m_sm->getSegmentsThatStartWith(current, segs);
        for (auto neighbor = segs.begin(); neighbor != segs.end(); neighbor++) {
            double tentative_gScore = *gScore.find(current) + distanceEarthMiles(current, neighbor->end);
            if (gScore.find(neighbor->end) == nullptr || tentative_gScore < *gScore.find(neighbor->end)) {
                // Records better paths than previous ones
                came_from.associate(neighbor->end, current);
                gScore.associate(neighbor->end, tentative_gScore);
                fScore.associate(neighbor->end, tentative_gScore + distanceEarthMiles(neighbor->end, end));
                openSet.push(LowestFScore(neighbor->end, *fScore.find(neighbor->end)));
            }
        }
    }
    //openSet empty without finding a path, no route
    cerr << "No Delivery" << endl;
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
