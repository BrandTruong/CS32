#include "provided.h"
#include <vector>
#include <string>
#include <iterator>
#include <iostream>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    void deliveryCommandGen(list<StreetSegment> toNextSpot, vector<DeliveryCommand>& commands, double& totalDistanceTravelled) const;
    const StreetMap* m_sm;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    m_sm = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    //Reset commands and totalDistanceTravelled first
    commands.clear();
    totalDistanceTravelled = 0;
    cerr << "Call generate Delivery Plan" << endl;
    //Optimize the route first
    DeliveryOptimizer optimized(m_sm);
    double x, y;
    vector<DeliveryRequest> optimized_deliveries = deliveries;
    optimized.optimizeDeliveryOrder(depot, optimized_deliveries, x, y); //x,y Not really used since crowDistance!=actual

    cerr << optimized_deliveries.size() << endl;

    //Inserts depot as a destination to the beginning and the end
    //Finds routes to every delivery
    PointToPointRouter routes(m_sm);
    list<StreetSegment> toNextSpot;
    double dist;

    GeoCoord startCoord = depot; //Begin at depot
    for (int i = 0; i < (int) optimized_deliveries.size(); i++) { //Through all delivery points
        cerr << endl << "Delivery number " << i + 1 << endl << endl;
        GeoCoord endCoord = optimized_deliveries[i].location;
        //Creates Route to location
        DeliveryResult result = routes.generatePointToPointRoute(startCoord, endCoord, toNextSpot, dist);
        if (result != DELIVERY_SUCCESS) { //Either BAD_COORD OR NO_ROUTE
            return result;
        }
        cerr << "Generate commands" << endl;
        //Generates commands
        deliveryCommandGen(toNextSpot, commands, totalDistanceTravelled);
        //Change endCoord to startCoord
        startCoord = endCoord;
        //Deliver command
        DeliveryCommand command = DeliveryCommand();
        command.initAsDeliverCommand(optimized_deliveries[i].item);
        commands.push_back(command);
    }
    //From last delivery location back to depot
    DeliveryResult result = routes.generatePointToPointRoute(startCoord, depot, toNextSpot, dist);
    if (result != DELIVERY_SUCCESS) { //Either BAD_COORD OR NO_ROUTE
        return result;
    }
    deliveryCommandGen(toNextSpot, commands, totalDistanceTravelled);
    cerr << "Reaches the end" << endl;
    return result; //DELIVERY_SUCCESS if reaches
}
void DeliveryPlannerImpl::deliveryCommandGen(list<StreetSegment> toNextSpot, vector<DeliveryCommand>& commands, double& totalDistanceTravelled) const {
    //Generate route to the next delivery location
    //Create commands to spot
     cerr << "The size is " << toNextSpot.size() << endl;
    for (auto it = toNextSpot.cbegin(); it != toNextSpot.cend(); it++) {
        bool skip = false;
        double distance = distanceEarthMiles(it->start, it->end);
        if (it != toNextSpot.begin()) { //Not the starting segment
            auto prevIt = it; //Previous iterator, checks to see if same street or new street
            prevIt--;
            //Combined distance
            //Same street name 
            if (prevIt->name == it->name) {
                commands.back().increaseDistance(distance);
                totalDistanceTravelled += distance;
                skip = true;
            }
            //Turn Command
            //Different street name
            else {
                DeliveryCommand command = DeliveryCommand();
                double angle = angleBetween2Lines(*prevIt, *it);
                string s = ""; //Default no turn
                if (angle >= 1 && angle < 180) { //Left
                    s = "left";
                }
                if (angle >= 180 && angle <= 359) { //Right
                    s = "right";
                }
                if (!s.empty()) { //If not empty then turn
                    command.initAsTurnCommand(s, it->name);
                    commands.push_back(command);
                }
            }
        }
        //Proceed Command
        if (!skip) {
            double angle = angleOfLine(*it);
            string dir;
            DeliveryCommand command = DeliveryCommand();
            //Calculates direction
            if (angle >= 0 && angle < 22.5) {
                dir = "east";
            }
            else if (angle >= 22.5 && angle < 67.5) {
                dir = "northeast";
            }
            else if (angle >= 67.5 && angle < 112.5) {
                dir = "north";
            }
            else if (angle >= 112.5 && angle < 157.5) {
                dir = "northwest";
            }
            else if (angle >= 157.5 && angle < 202.5) {
                dir = "west";
            }
            else if (angle >= 202.5 && angle < 247.5) {
                dir = "southwest";
            }
            else if (angle >= 247.5 && angle < 292.5) {
                dir = "south";
            }
            else if (angle >= 292.5 && angle < 337.5) {
                dir = "southeast";
            }
            else {
                dir = "east";
            }
            //Pushes command
            command.initAsProceedCommand(dir, it->name, distance);
            commands.push_back(command);
            totalDistanceTravelled += distance;
        }
        
    }
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
