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
    vector<StreetSegment> streets;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

// TODO: write ifstream to load and then parse through file and fill in
// 1. string streetName
// 2. Geosegment segment
// 3. vector<Attraction> attractions
bool MapLoaderImpl::load(string mapFile)    // TO ASK: does mapFile have the .txt extension in the string or do I have to add this
{
    ifstream infile("/Users/austinguo550/Documents/CS32 2017/BruinNav/BruinNav/" + mapFile);    // TODO: remove the path name, because linux g++ will search the current directory. TO ASK: do we need to remove this path??
    if (!infile) {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    string line = "";
    int numStreetOn = 0;
    int numAttractions = 0;
    while (getline(infile, line)) { // for each street
        
        // read street name
        streets[numStreetOn].streetName = line;
        
        // read coordinates
        if (getline(infile, line)) {
            // line is now startLat, startLong, endLat, endLong
            vector<string> tempCoordsHolder;
            int tempCoordNum = 0;
            string tempCoord;
            while (infile >> tempCoord) {   // reads the next word into tempCoord excluding spaces
                if (tempCoordNum == 0 || tempCoordNum == 2) {
                    tempCoord = tempCoord.substr(0, tempCoord.size()-1);    // need to get rid of the comma for latitude values
                }
                tempCoordsHolder[tempCoordNum] = tempCoord;
                tempCoordNum++;
            }
//            manually reading in, if above works then use it
//            for (int i = 0; i < line.size(); i++) {
//                //put startLat, startLong, endLat, endLong into the tempCoordsHolder
//                if (line[i] == ' ') {
//                    tempCoordsHolder[tempCoordNum] = tempCoord;
//                    tempCoordNum++;
//                    tempCoord = "";
//                }
//                else {
//                    tempCoord += line[i];   // add the character
//                }
//            }
            // set the geosegment geocoords
            //start
            streets[numStreetOn].segment.start.latitudeText = tempCoordsHolder[0];
            streets[numStreetOn].segment.start.longitudeText = tempCoordsHolder[1];
            streets[numStreetOn].segment.start.latitude = stoi(tempCoordsHolder[0]);
            streets[numStreetOn].segment.start.longitude = stoi(tempCoordsHolder[1]);
            
            //end
            streets[numStreetOn].segment.start.latitudeText = tempCoordsHolder[2];
            streets[numStreetOn].segment.start.longitudeText = tempCoordsHolder[3];
            streets[numStreetOn].segment.start.latitude = stoi(tempCoordsHolder[2]);
            streets[numStreetOn].segment.start.longitude = stoi(tempCoordsHolder[3]);
        }
        
        // read num attractions
        int k;
        string attraction;
        string attractionLat;
        string attractionLong;
        if (infile >> k) {
            
            infile.ignore(10000, '\n');
            for (int i = 0; i < k; i++) {   // for each attraction
                
                char c;
                while (infile >> c && c != '|') {
                    attraction += c;
                }
                streets[numStreetOn].attractions[i].name = attraction;  // name each of the attractions
                
                //reading the latitude/longitude of attraction
                //when it exits the while loop, it'll be on a digit
                if (infile >> attractionLat) {
                    attractionLat = attractionLat.substr(0, attractionLat.size()-1); //get rid of the comma
                    streets[numStreetOn].attractions[i].geocoordinates.latitudeText = attractionLat;
                    streets[numStreetOn].attractions[i].geocoordinates.latitude = stoi(attractionLat);
                }
                
                if (infile >> attractionLong) {
                    streets[numStreetOn].attractions[i].geocoordinates.longitudeText = attractionLong;
                    streets[numStreetOn].attractions[i].geocoordinates.longitude = stoi(attractionLong);
                }
                
            }
            
        }
        
    }
    
    return true;  // This compiles, but may not be correct
}

size_t MapLoaderImpl::getNumSegments() const
{
	return streets.size();
}


// TODO: cannot write an assignment operator, so just edit here/ make a helper function actually
bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum > 0 || segNum < getNumSegments()) {
        seg.streetName = streets[segNum].streetName;    // strings can be reassigned
        seg.segment = streets[segNum].segment;  // do Geosegments have assignment operators?
        seg.attractions = streets[segNum].attractions;  // do vectors have assignment operators?
        // address of previous seg isn't changed, no new one is created, old one isn't deleted
        //seg = streets[segNum];    // CANNOT DO THIS BECAUSE NO ASSIGNMENT OPERATOR??
        return true;
    }
    return false;   // out of bounds
}

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
