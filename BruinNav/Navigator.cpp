#include "provided.h"
#include <string>
#include <vector>
#include <queue>
#include "MyMap.h"
#include "support.h"
#include <algorithm>
#include <cmath>
using namespace std;




class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
    
private:
    MapLoader ml;
    SegmentMapper segMap;
    AttractionMapper attractionMapper;
    
    vector<GeoCoord> pathFind(string begin, string destination) const;
    
    struct Pair {
        vector<StreetSegment> streetsToTraverse;
        vector<double> distancesToTraverse;
    };
    Pair* getStreetsAndDistances(vector<GeoCoord> geoCoords) const;
    vector<NavSegment> generateSegments(Pair* data) const;
    
    string turnDirection(double angle) const;
};

// NAVIGATE ORDER OF USE
// pathFind
// getStreetsAndDistances
// generateSegments


NavigatorImpl::NavigatorImpl()
{

}

NavigatorImpl::~NavigatorImpl()
{
}

// TODO
bool NavigatorImpl::loadMapData(string mapFile)
{
    ml.load(mapFile);
    segMap.init(ml);
    attractionMapper.init(ml);
    return true;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    GeoCoord temp;
    if (!attractionMapper.getGeoCoord(start, temp)) {
        return NAV_BAD_SOURCE;
    }
    if (!attractionMapper.getGeoCoord(end, temp)) {
        return NAV_BAD_DESTINATION;
    }
    
    vector<GeoCoord> pathToDisplay = pathFind(start, end);
    if (pathToDisplay.empty()) {
        return NAV_NO_ROUTE;  // This compiles, but may not be correct
    }
    
    Pair* streetsAndDistances = getStreetsAndDistances(pathToDisplay);
    
    directions = generateSegments(streetsAndDistances); // convert the streetsegments and distances to navsegment directions
    
//    cerr << "IMPORTANNTTTTTTTTTTTT" << directions.size();
    
//    // testing print out streets visited
//    for (int i = 0; i < streetsAndDistances->streetsToTraverse.size(); i++) {
//        cerr << "Travel " << streetsAndDistances->distancesToTraverse[i] << " miles on " << streetsAndDistances->streetsToTraverse[i].streetName << " (" << streetsAndDistances->streetsToTraverse[i].segment.start.latitudeText << ", " << streetsAndDistances->streetsToTraverse[i].segment.start.longitudeText << ") " << " (" << streetsAndDistances->streetsToTraverse[i].segment.end.latitudeText << ", " << streetsAndDistances->streetsToTraverse[i].segment.end.longitudeText << ") " << endl;
//    }
    
    delete streetsAndDistances;
    // TODO: turn streetAndDistances into directions of navSegments
    return NAV_SUCCESS;
	
}








string NavigatorImpl::turnDirection(double angle) const {
    if (angle < 180) {
        return "left";
    }
    // otherwise angle is greater than or equal to 180
    return "right";
}



vector<GeoCoord> NavigatorImpl::pathFind(string begin, string destination) const {
    
    // write helper function to pass in GeoCoords
    GeoCoord start, goal;
    attractionMapper.getGeoCoord(begin, start);
    attractionMapper.getGeoCoord(destination, goal);
    
    
    priority_queue<NavNode> untriedPath;
    MyMap<GeoCoord, double> nodesVisited;
    
    NavNode* startNode;
    vector<GeoCoord> pathBeforeStart;   // before starting, the path is empty
    startNode = new NavNode(start, 0, 0, pathBeforeStart);
    untriedPath.push(*startNode);
    delete startNode;
    
//    cerr << start.latitude << ", " << start.longitude << " to " << goal.latitude << ", " << goal.longitude << endl;
//    cerr << "distance is " <<distanceEarthMiles(start, goal) << endl;
    
    
    
    while (!untriedPath.empty())    // there's still something in the queue
    {
        NavNode parentNode = untriedPath.top();
        //////////////////////////////////////////////////////
//        cerr << " Entered " << parentNode.getPriority() << endl;
        //////////////////////////////////////////////////////
        untriedPath.pop();
        nodesVisited.associate(parentNode.getGeoCoord(), parentNode.getPriority()); // won't matter if i double associate this geocoord as visited, since it's priority won't change
        
//        if (openNodes.find(parentNode.getGeoCoord()) != nullptr) {
//            //GeoCoord* coordToPush = new GeoCoord;
//            //*coordToPush = parentNode.getGeoCoord();
//            //openNodes.associate(*coordToPush, -1);
//            openNodes.associate(parentNode.getGeoCoord(), -1);  // not the problem with "pointer being freed was not allocated"
//        }
        // TODO check to see if parent is end
        
        vector<StreetSegment> successors;
        successors = segMap.getSegments(parentNode.getGeoCoord());

        
        // check to see if we've reached the goal
        if (parentNode.getGeoCoord() == goal) {   // we've found the right path!
//            cerr << "FOUND!!" << endl;
//            for (int m = 0; m < parentNode.getPath().size(); m++) {
//                cerr << (parentNode.getPath())[m].latitudeText << "," << (parentNode.getPath())[m].longitudeText << endl;
//            }
            return parentNode.getPath();
        }
        
        // have a bunch of street segments
        for (int i = 0; i < successors.size(); i++) {
            
            
//            ///////////////////////////////////////////////////
//            // TESTING
//            if (parentNode.getGeoCoord().latitudeText == "34.0627650")
//                cerr << "HIII " << successors[i].segment.start.latitudeText << ", " << successors[i].segment.start.latitudeText<<  endl;
//            //////////////////////////////////////////////////////
            
            // seeing the linkage of streets tested
//            cerr << successors[i].streetName << " (" << successors[i].segment.start.latitudeText << ", " << successors[i].segment.start.longitudeText << ") " << " (" << successors[i].segment.end.latitudeText << ", " << successors[i].segment.end.longitudeText << ") " << endl;
            
            // the successive segments produced could have either their end or start connected to the parent node
            GeoCoord sideToTest;
            
            for (int k = 0; k < successors[i].attractions.size(); k++) {
                if (successors[i].attractions[k].geocoordinates == goal) {
                    // push attractions
                    NavNode* attractionNode = new NavNode(successors[i].attractions[k].geocoordinates, parentNode.getLevel(), parentNode.getPriority(), parentNode.getPath());
                    attractionNode->nextLevel(&parentNode);
                    attractionNode->updatePriority(goal);
                    
                    double*  priorPriority = nodesVisited.find(attractionNode->getGeoCoord());
                    if (priorPriority == nullptr || (priorPriority != nullptr && attractionNode->getPriority() < *priorPriority)) {   // it hasn't been visited, or it has been and the new priority is less than
//                        cerr << "a to b is " << distanceEarthMiles(parentNode.getGeoCoord(), attractionNode->getGeoCoord()) << endl;
                        untriedPath.push(*attractionNode);   // we'll try to visit it next time: it's pushed to the priorityqueue
//                        nodesVisited.associate(attractionNode->getGeoCoord(), attractionNode->getPriority());
                        delete attractionNode;
                    }
                }
            }
            
            if (successors[i].segment.end == parentNode.getGeoCoord()) {
                sideToTest = successors[i].segment.start;
            }
            else if (successors[i].segment.start == parentNode.getGeoCoord()) {
                sideToTest = successors[i].segment.end;
            }
            else {  // only occurs at the start or end node
                
                
                
                sideToTest = successors[i].segment.start;
                
                NavNode*  successor = new NavNode(sideToTest, parentNode.getLevel(), parentNode.getPriority(), parentNode.getPath());
                
                // otherwise
                // update the node's level and get its priority
                successor->nextLevel(&parentNode);
                successor->updatePriority(goal);
                
                // nodes visited
                double*  testing = nodesVisited.find(successor->getGeoCoord());  // TODO: may have to change the name of the variable
                if (testing == nullptr || (testing != nullptr && successor->getPriority() < *testing)) {   // it hasn't been visited, or it has been and the new priority is less than something already visited
//                    cerr << "a to b is " << distanceEarthMiles(parentNode.getGeoCoord(), successor->getGeoCoord()) << endl;
                    untriedPath.push(*successor);   // we'll try to visit it next time: it's pushed to the priority queue
//                    nodesVisited.associate(successor->getGeoCoord(), successor->getPriority());
                }
                
                delete successor;   // TODO: pray that it pushes a copy of the node to the priority queue\
                
                sideToTest = successors[i].segment.end;
                
                NavNode* successor2 = new NavNode(sideToTest, parentNode.getLevel(), parentNode.getPriority(), parentNode.getPath());
                
                // otherwise
                // update the node's level and get its priority
                successor2->nextLevel(&parentNode);
                successor2->updatePriority(goal);
                
                // nodes visited
                double* testing2 = nodesVisited.find(successor2->getGeoCoord());  // TODO: may have to change the name of the variable
                if (testing2 == nullptr || (testing2 != nullptr && successor2->getPriority() < *testing2)) {   // it hasn't been visited, or it has been and the new priority is less than
                    //                    cerr << "a to b is " << distanceEarthMiles(parentNode.getGeoCoord(), successor2->getGeoCoord()) << endl;
                    untriedPath.push(*successor2);   // we'll try to visit it next time: it's pushed to the priority queue
//                    nodesVisited.associate(successor2->getGeoCoord(), successor2->getPriority());
                }
                
                delete successor2;   // TODO: pray that it pushes a copy of the node to the priority queue
                
                
                
                continue;
            }
                
            NavNode* successor = new NavNode(sideToTest, parentNode.getLevel(), parentNode.getPriority(), parentNode.getPath());
            // update the node's level and get its priority
            successor->nextLevel(&parentNode);
            successor->updatePriority(goal);
        
            // nodes visited
            double*  testing = nodesVisited.find(successor->getGeoCoord());  // TODO: may have to change the name of the variable
            
            if (testing == nullptr || (testing != nullptr && successor->getPriority() < *testing)) {   // it hasn't been visited, or it has been and the new priority is less than
                
//                ///////////////////////////////////////////////////
//                // TESTING
//                if (successor->getGeoCoord().latitudeText == "34.0627650")
//                    cerr << "HIII" << endl;
//                //////////////////////////////////////////////////////
//                cerr << "a to b is " << distanceEarthMiles(parentNode.getGeoCoord(), successor->getGeoCoord()) << endl;
                untriedPath.push(*successor);   // we'll try to visit it next time: it's pushed to the priority queue
//                nodesVisited.associate(successor->getGeoCoord(), successor->getPriority());
            }
            
            delete successor;   // TODO: pray that it pushes a copy of the node to the priority queue
        }
    }
    
//    cerr << "Couldn't find " << endl;
    vector<GeoCoord> empty;
    return empty;
}


NavigatorImpl::Pair* NavigatorImpl::getStreetsAndDistances(vector<GeoCoord> geoCoords) const {
    vector<StreetSegment> resultSegments;
    vector<double> resultDistances;
    
//    for (int i = 0; i < geoCoords.size();i++) {
////        cerr << i << endl;
//        //cerr << distanceEarthMiles(geoCoords[i], geoCoords[i+1]) << endl;
//    }
    
    
    // debug: i is to traverse geocoords, j is to traverse successor streetsegments off geocoord
    vector<StreetSegment> tempSegs; vector<StreetSegment> tempSegs2;
    for (int i = 0; i < geoCoords.size() - 1; i++) {    // last geocoord won't map a street to anything, prevents out of bounds access
        tempSegs = segMap.getSegments(geoCoords[i]);   // get the street segments off this geocoord
        tempSegs2 = segMap.getSegments(geoCoords[i+1]);
        
        bool skip = false;
        for (int j = 0; j < tempSegs.size(); j++) {
            if (skip) break;
            GeoCoord a = geoCoords[i];
            GeoCoord b = geoCoords[i+1];
            
            for (int k = 0; k < tempSegs2.size(); k++) {
                if (tempSegs[j] == tempSegs2[k]) {
                    // generate my own street segment with the correct direction
                    StreetSegment tempSegmentToPush;
                    tempSegmentToPush.streetName = tempSegs[j].streetName;
                    GeoSegment tempGeoSegToPush;
                    tempGeoSegToPush.start = a;
                    tempGeoSegToPush.end = b;
                    tempSegmentToPush.segment = tempGeoSegToPush;
                    // TODO: add attractions
                    
                    resultSegments.push_back(tempSegmentToPush);
                    // already pushed back a street
                    skip = true;
                }
            }
            
            
            // there must be a street segment that connects a and b if this method is called
            // either by start and end
            //            if ((tempSegs[j].segment.start == a && tempSegs[j].segment.end == b)||(tempSegs[j].segment.end == a && tempSegs[j].segment.start == b))
            //            {
            //                resultSegments.push_back(tempSegs[j]);
            //            }
            //            // or by attraction and start or attraction and end (only occurs in first and last geocoord/all other geocoords will be the start or ends of streets, cannot be connected by attractions)
            //            if (i == 0 || i == geoCoords.size()-2) {
            //                for (Attraction attraction : tempSegs[j].attractions) {
            //                    if (attraction.geocoordinates == a && i == 0) {   // its attraction is the starting geocoord
            //                        resultSegments.push_back(tempSegs[j]);
            //                    }
            //                    else if (attraction.geocoordinates == b && i == geoCoords.size()-2)  { // its attraction is the ending geocoord
            //                        resultSegments.push_back(tempSegs[j]);
            //                    }
            //                }
            //            }
        }
        
        resultDistances.push_back(distanceEarthMiles(geoCoords[i], geoCoords[i+1]));    // distance is always between a and b
    }
    Pair* result = new Pair;
    result->streetsToTraverse = resultSegments;
    result->distancesToTraverse = resultDistances;
    return result;
}


vector<NavSegment> NavigatorImpl::generateSegments(Pair* data) const {
    vector<NavSegment> result;
    vector<StreetSegment> streets = data->streetsToTraverse;
    vector<double> distanceVec = data->distancesToTraverse;
//    cerr << "DISTANCE PAY ATTENTION " << distanceVec[0] << endl;
//    cerr << "DISTANCE SIZE PAY ATT " << distanceVec.size() << endl;
//    cerr << "STREETS SIZE " << streets.size() << endl;
    string prevStreet = streets[0].streetName;
    GeoSegment geoSeg;
    double distance = 0.0;
    for (int i = 0; i < streets.size(); i++) {
        if (streets[i].streetName != prevStreet) {
            // create a TURN NavSeg
            NavSegment segment(turnDirection(angleBetween2Lines(geoSeg, streets[i].segment)), streets[i].streetName);  // passing in the previous geosegment and this geosegment
            result.push_back(segment);
//            cerr << "PUSHHHHHHHHHHHHH TURN" << endl;
            //geoSeg = streets[i].segment;

            prevStreet = streets[i].streetName;
            
            //testing
//            cerr << "WHY" << distance << endl;
            distance = 0;
        }
        
        // create a PROCEED NavSeg
        geoSeg = streets[i].segment;
        // initialize the direction, street name, distance, and geoSegment. m_command is already made
        NavSegment segment(directionOfLine(geoSeg), streets[i].streetName, distanceVec[i], geoSeg);
        
        //testing
        distance = distance + distanceVec[i];
        
//        cerr << "PUSHHHHHHHHHHHHH" << endl;
        result.push_back(segment);
        
        
        //        if (streets[i].streetName == prevStreet) {
        //            // create a PROCEED NavSeg
        //            geoSeg = streets[i].segment;
        //            // initialize the direction, street name, distance, and geoSegment. m_command is already made
        //            NavSegment segment(directionOfLine(geoSeg), streets[i].streetName, distanceVec[i], geoSeg);
        //
        //            //testing
        //            distance = distance + distanceVec[i];
        //
        //            result.push_back(segment);
        //        }
        //        else {  // the street ended
        //            // create a TURN NavSeg
        //            NavSegment segment(turnDirection(angleBetween2Lines(geoSeg, streets[i].segment)), streets[i].streetName);  // passing in the previous geosegment and this geosegment
        //            result.push_back(segment);
        //            geoSeg = streets[i].segment;
        //            prevStreet = streets[i].streetName;
        //            i--;
        //
        //            //testing
        //            cerr << "WHY" << distance << endl;
        //            distance = 0;
        //        }
    }
    return result;
}





// when trying to get the streetsegments from the geocoords solution A,B, C, D, E,... N, just call getSegments on A, check which street has an end at B, and repeat for B




//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
