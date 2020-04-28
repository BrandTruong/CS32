#include "provided.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    double crowDistance(GeoCoord depot, const vector<DeliveryRequest> deliveries) const;
    inline int randInt(int min, int max) const
    {
        if (max < min)
            std::swap(max, min);
        static std::random_device rd;
        static std::default_random_engine generator(rd());
        std::uniform_int_distribution<> distro(min, max);
        return distro(generator);
    }
    inline void swap(int curr, int rand, vector<DeliveryRequest>& deliveries) const
    {
        DeliveryRequest temp(deliveries[curr].item, deliveries[curr].location);
        deliveries[curr] = deliveries[rand];
        deliveries[rand] = temp;
    }
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    //Calculates oldCrow
    oldCrowDistance = crowDistance(depot, deliveries);

    //Pesudo-random based on simulated annealing
    int n = 0;
    int heat = (int) deliveries.size() / 2;
    double currentDist = oldCrowDistance;
    while (n < (int) deliveries.size()) {
        for (int i = 0; i < (int)deliveries.size(); i++) { //Swaps around current i with a random other deliveryrequest
            int curr = n % deliveries.size();
            int rand = randInt(0, deliveries.size() - 1);
            swap(curr, rand, deliveries);
            newCrowDistance = crowDistance(depot, deliveries);
            if (newCrowDistance - currentDist > 0 && randInt(0, deliveries.size()) >= heat) { //checks if the permmutation is more efficient
                swap(rand, curr, deliveries); //if not efficient then chance to swap back to original or keep this permutation
            }
            else {
                currentDist = newCrowDistance; 
            }
        }
        heat /= 2; //change heat so less likely to choose a random permutation that is less efficient as we loop more
        n++;
    }
    std::cerr << "oldCrowDistance is " << oldCrowDistance << endl << "newCrowDistance is " << newCrowDistance << endl;
}

double DeliveryOptimizerImpl::crowDistance(GeoCoord depot, const vector<DeliveryRequest> deliveries) const
{
    GeoCoord coord1 = depot;
    double dist = 0;
    //Calculates oldCrowDistance
    for (int i = 0; i < (int)deliveries.size(); i++) {
        GeoCoord coord2 = deliveries[i].location;
        double currentDist = distanceEarthMiles(coord1, coord2);
        dist += currentDist;
        coord1 = coord2;
    }
    dist += distanceEarthMiles(depot, coord1);
    return dist;
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
