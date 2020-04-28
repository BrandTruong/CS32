#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    void findHashNode(GeoCoord startCoord, GeoCoord endCoord, string s);
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_segments;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    m_segments.reset(); //makes sure hashmap is empty
    ifstream infile(mapFile);
    if (!infile)		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    std::string s;  //street name in here
    // getline returns infile; the while tests its success/failure state
    while (getline(infile, s)) //This will reach O(N) despite the nested loop since the loop takes in the succeeding lines of Coords
    {
        cerr << "Getting street " << s << endl;
        int numsSeg;
        infile >> numsSeg;
        infile.ignore(10000, '\n');
        for (int i = 0; i < numsSeg; i++) { //This will always be lower than N and will reduce the amount of times the getline is called in the while loop
            //start Coordinates
            string lat, lon;
            infile >> lat;
            infile >> lon;
            GeoCoord startCoord(lat, lon);
            //end Coordinates
            infile >> lat;
            infile >> lon;
            GeoCoord endCoord(lat, lon);
            findHashNode(startCoord, endCoord, s); //either associates (creates) a HashNode if not found or pushes to the vector the segment
            findHashNode(endCoord, startCoord, s);
            infile.ignore(10000, '\n'); //Proceeds to next line
        }
        cerr << "Obtained numsSeg " << numsSeg << endl;
    }
    return true;  //Read file
}
void StreetMapImpl::findHashNode(GeoCoord startCoord, GeoCoord endCoord, string s) { //Pushes to vector if found, otherwise creates a HashNode
    vector<StreetSegment>* vptr = m_segments.find(startCoord);
    if (vptr == nullptr) { //creates a HashNode for not found start coord
        vector<StreetSegment> sg1;
        sg1.push_back(StreetSegment(startCoord, endCoord, s));
        m_segments.associate(startCoord, sg1);
    }
    else { //Pushes new value in address
        vptr->push_back(StreetSegment(startCoord, endCoord, s));
    }
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    const vector<StreetSegment>* vptr = m_segments.find(gc); //Finds start Coordinate
    if (vptr != nullptr) {
        segs.clear(); //start with empty vector if found
        for (auto ptr = vptr->cbegin(); ptr != vptr->cend(); ptr++) { //Pushes entire vector to segs
            segs.push_back(*ptr);
        }
        return true;
    }
    return false;  //unchanged vector, no such coord
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}

