#include "provided.h"
#include "MyMap.h"
#include <string>
#include "support.h"
#include <algorithm>
using namespace std;


class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
    
private:
    MyMap<string, GeoCoord> attractionMap;
    int numAttractions;
    string convertToLower(const string str) const;
};

// TODO
AttractionMapperImpl::AttractionMapperImpl()
{
    numAttractions = 0;
}

// TODO
AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    StreetSegment temp;
    for (int i = 0; i < ml.getNumSegments(); i++) {
        if (ml.getSegment(i, temp)) {
            for (int j = 0; j < temp.attractions.size(); j++) {
                string attractionNameToAssociate = convertToLower(temp.attractions[j].name);
                attractionMap.associate(attractionNameToAssociate, temp.attractions[j].geocoordinates);
                numAttractions++;
                
                // TODO: comment out: just a test to see if there's any empty string geocoordinates
//                if (temp.attractions[j].geocoordinates.latitudeText == "" || temp.attractions[j].geocoordinates.longitudeText == "") {
//                    cerr << "We have a problem in AttractionMapper" << endl;
//                }
            }
        }
        
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    string tempAttraction = convertToLower(attraction);
    const GeoCoord* temp = attractionMap.find(tempAttraction);
    if (temp != nullptr) {
        gc = *temp;
        return true;
    }
    return false;
}

// added
string AttractionMapperImpl::convertToLower(const string str) const {
    string result = "";
    for (int i = 0; i < str.size(); i++) {
        result += tolower(str[i]);
    }
    return result;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
