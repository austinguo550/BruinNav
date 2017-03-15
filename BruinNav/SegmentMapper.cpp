#include "provided.h"
#include "MyMap.h"
#include <vector>
#include "support.h"
#include <algorithm>
using namespace std;


class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
private:
    //bool compareCoords(const GeoCoord& geo1, const GeoCoord& geo2) const;
    
    MyMap<GeoCoord, vector<StreetSegment>> segmentMaps;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    
    for (int i = 0; i < ml.getNumSegments(); i++) {
        StreetSegment temp;
        if (ml.getSegment(i, temp)) {
            
            // check the attractions
            for (int k = 0; k < temp.attractions.size(); k++) {
                //GeoCoord aCoord = temp.attractions[i].geocoordinates;
                
                // TODO: comment out: just checking if there's any empty string geocoordinates
                if (temp.attractions[k].geocoordinates.latitudeText == "" || temp.attractions[k].geocoordinates.longitudeText == "") {
                    cerr << "We have a problem in SegmentMapper: associating by attractions: failed at " << temp.streetName << " "  << temp.attractions.size() << " " << temp.attractions[i].geocoordinates.latitudeText <<" " << temp.attractions[i].geocoordinates.longitudeText << " "<< temp.attractions[i].geocoordinates.latitude << " "<< temp.attractions[i].geocoordinates.longitude << endl;
                }
                
                
                vector<StreetSegment>* ptrA = segmentMaps.find(temp.attractions[k].geocoordinates);
                if (ptrA != nullptr) {   // there is this coord already, don't have to check if the streetname is already there because it cannot be: cannot have repeat street segments in the vector in maploader, and cannot have repeat attractions inside each streetsegment (must have unique attractions within one unique streetseg)
                    (*ptrA).push_back(temp); // push this current streetSegment onto the vector of the temporary coordinate
                }
                else {
                    vector<StreetSegment> addStreetSeg; // the geocoord doesn't exist, create a new street segment vector and associate it with the geocoord
                    addStreetSeg.push_back(temp);
                    segmentMaps.associate(temp.attractions[k].geocoordinates, addStreetSeg);
                }
            }
            
            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////////
            
            
            
            // checking the start coordinate
            //GeoCoord tempCoord = temp.segment.start;
            
            // TODO: comment out: just checking to see if there's any empty geocoordinates
            if (temp.segment.start.latitudeText == "" || temp.segment.start.longitudeText == "" || temp.segment.end.latitudeText == "" || temp.segment.end.longitudeText == "") {
                cerr << "We have a problem associating by start or end" << endl;
            }
            
            
            // find in map
            vector<StreetSegment>* ptr = segmentMaps.find(temp.segment.start);
            if (ptr != nullptr) {   // there is this coord already
                // find in vector at geocoord
                if(find((*ptr).begin(), (*ptr).end(), temp) != (*ptr).end()){
                    // if it's in the vector, do nothing
                }
                else {
                    (*ptr).push_back(temp); // push this current streetSegment onto the vector of the temporary coordinate
                }
            }
            else {
                vector<StreetSegment> addStreetSeg; // the geocoord doesn't exist, create a new street segment vector and associate it with the geocoord
                addStreetSeg.push_back(temp);
                segmentMaps.associate(temp.segment.start, addStreetSeg);
            }
            
            // checking the end coordinate
            //GeoCoord tempCoordEnd = temp.segment.end;
            
            // find in map
            vector<StreetSegment>* ptr2 = segmentMaps.find(temp.segment.end);
            if (ptr2 != nullptr) {   // there is this coord already
                // find in vector at geocoord
                if (find((*ptr2).begin(), (*ptr2).end(), temp) != (*ptr2).end()){
                    // if it's in the vector, do nothing
                }
                else {
                    (*ptr2).push_back(temp); // push this current streetSegment onto the vector of the temporary coordinate
                }
            }
            else {
                vector<StreetSegment> addStreetSeg2; // the geocoord doesn't exist, create a new street segment vector and associate it with the geocoord
                addStreetSeg2.push_back(temp);
                segmentMaps.associate(temp.segment.end, addStreetSeg2);
            }
        }
        //cerr << temp.streetName << " " << temp.segment.start.latitude << " " << temp.segment.end.longitude << endl;   // TODO: only for testing
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
    if (segmentMaps.find(gc) == nullptr) {
        return segments;
    }
    segments = *(segmentMaps.find(gc));
    //cerr << segmentMaps.size();
	return segments;  // This compiles, but may not be correct
}

//bool SegmentMapperImpl::compareCoords(const GeoCoord& geo1, const GeoCoord& geo2) const {
//    return (geo1.latitudeText == geo2.latitudeText) && (geo1.longitudeText == geo2.longitudeText);  //(geo1.latitude == geo2.latitude) && (geo1.longitude == geo2.longitude) &&
//}






//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
