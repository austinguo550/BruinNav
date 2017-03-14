#include "provided.h"
#include <string>
///////////////////////////////////////////////////
// ADDED
#include <vector>
#include <iostream>
#include <fstream>

///////////////////////////////////////////////////
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    
private:
    //double stringToDouble(const string str);
    string removeWhiteSpace(const string str);
    
    vector<StreetSegment> streets;
};

// TODO
MapLoaderImpl::MapLoaderImpl()
{
}

// TODO
MapLoaderImpl::~MapLoaderImpl()
{
}


bool MapLoaderImpl::load(string mapFile)    // TO ASK: does mapFile have the .txt extension in the string or do I have to add this
{
    ifstream infile(mapFile);
    if (!infile) {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    string line = "";
    
    while (getline(infile, line)) { // for each street
        StreetSegment oneStreet;
        
        // read street name
        oneStreet.streetName = line;
        
        char badChar;
        
        // read coordinates
        getline(infile, line, ',');
        line = removeWhiteSpace(line);
        oneStreet.segment.start.latitudeText = line;
        oneStreet.segment.start.latitude = stod(line);
        
        while (isspace(infile.peek())) {
            infile.get(badChar);
        }
        getline(infile, line, ' ');
        line = removeWhiteSpace(line);
        oneStreet.segment.start.longitudeText = line;
        oneStreet.segment.start.longitude = stod(line);
        
        getline(infile, line, ',');
        line = removeWhiteSpace(line);
        oneStreet.segment.end.latitudeText = line;
        oneStreet.segment.end.latitude = stod(line);
        
        while (isspace(infile.peek())) {
            infile.get(badChar);
        }
        getline(infile, line);
        line = removeWhiteSpace(line);
        oneStreet.segment.end.longitudeText = line;
        oneStreet.segment.end.longitude = stod(line);

        
        // read num attractions
        int k;
        string attraction;
        string attractionLat;
        string attractionLong;
        if (infile >> k) {
            infile.ignore(10000, '\n');
            
            for (int i = 0; i < k; i++) {   // for each attraction
                
                Attraction att;
                
                infile.unsetf(ios_base::skipws);    // make it read whitespace
                
                getline(infile, attraction, '|');
                att.name = attraction;  // name each of the attractions
                
                infile.setf(ios_base::skipws);  // make it skip whitespace again
                
                //reading the latitude/longitude of attraction
                //when it exits the while loop, it'll be on a digit
                getline(infile, attractionLat, ',');
                removeWhiteSpace(attractionLat);
                att.geocoordinates.latitudeText = attractionLat;
                att.geocoordinates.latitude = stod(attractionLat);
                
                while (isspace(infile.peek())) {
                    infile.get(badChar);
                }
                getline(infile, attractionLong);
                removeWhiteSpace(attractionLong);
                att.geocoordinates.longitudeText = attractionLong;
                att.geocoordinates.longitude = stod(attractionLong);
                
                oneStreet.attractions.push_back(att);
            }
        }
        
        
        // TODO: comment out: only to test if we insert a GeoCoord with empty strings
        if (oneStreet.segment.start.latitudeText == "" || oneStreet.segment.start.longitudeText == "" || oneStreet.segment.end.latitudeText == "" || oneStreet.segment.end.longitudeText == "") {
            cerr << "We have a problem" << endl;
        }
        
        streets.push_back(oneStreet);
    }
    
//    // TODO: delete this comment: just to test if I messed up pushing streetsegments: sometimes their attractions are "" geocoord
//    for (int i = 0; streets[segNum].attractions.size(); i++) {
//        if (streets[segNum].attractions[i].geocoordinates.latitudeText == "" || streets[segNum].attractions[i].geocoordinates.longitudeText == "") {
//            cerr << "We have a problem in MAPLOADER the original streets at segnum " << segNum << endl;
//        }
//    }
    
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return streets.size();
}


// WORKS!!! DON'T MESS WITH IT NOW!!
bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum > 0 || segNum < getNumSegments()) {
        seg.streetName = streets[segNum].streetName;    // strings can be reassigned
        seg.segment = streets[segNum].segment;
        seg.attractions = streets[segNum].attractions;
        
        // address of previous seg isn't changed, no new one is created, old one isn't deleted
        //seg = streets[segNum];    // CANNOT DO THIS BECAUSE NO ASSIGNMENT OPERATOR??
        return true;
    }
    return false;   // out of bounds
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// functions I created

string MapLoaderImpl::removeWhiteSpace(const string str) {
    string result = "";
    for (int i = 0; i < str.size(); i++) {
        if (!isspace(str[i])) {
            result += str[i];
        }
    }
    return result;
}

//// TO ASK: precision?? it has precision of 34.0434156000000029962 // the real value is 34.0434156
//double MapLoaderImpl::stringToDouble(const string str) {
//    double result;
//    int decimalPos = 0;
//    
//    result = stoi(str);
//    // parse until we find the decimal
//    
//    for (; decimalPos < str.size() && str[decimalPos] != '.'; decimalPos++) {} // move to the decimal position
//    for (int i = decimalPos + 1, place = 1; i < str.size(); i++, place++) { // add on the decimal values
//        double decimalPlaceMult = 1;
//        for (int j = 0; j < place; j++) {
//            decimalPlaceMult *= 0.10;
//        }
//        result += (decimalPlaceMult * (str[i] - '0'));
//    }
//    
//    return result;
//}




//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
