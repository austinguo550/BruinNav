#include "provided.h"
#include <string>
#include <vector>
#include <queue>
#include "MyMap.h"
#include "support.h"
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
};



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
    return false;  // This compiles, but may not be correct
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
    
    // turn directions to pathToDisplay, except pathToDisplay needs to use navSegments
    return NAV_SUCCESS;
	
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
    
    cerr << start.latitude << ", " << start.longitude << " to " << goal.latitude << ", " << goal.longitude << endl;
    cerr << "distance is " <<distanceEarthMiles(start, goal) << endl;
    
    
    
    while (!untriedPath.empty())    // there's still something in the queue
    {
        NavNode parentNode = untriedPath.top();
        //////////////////////////////////////////////////////
        cerr << " Entered " << parentNode.getPriority() << endl;
        //////////////////////////////////////////////////////
        untriedPath.pop();
        nodesVisited.associate(parentNode.getGeoCoord(), parentNode.getPriority());
        
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
            cerr << "FOUND!!" << endl;
            return parentNode.getPath();
        }
        
        // have a bunch of street segments
        for (int i = 0; i < successors.size(); i++) {
            
            
            ///////////////////////////////////////////////////
            // TESTING
            if (parentNode.getGeoCoord().latitudeText == "34.0627650")
                cerr << "HIII " << successors[i].segment.start.latitudeText << ", " << successors[i].segment.start.latitudeText<<  endl;
            //////////////////////////////////////////////////////
            
            // seeing the linkage of streets tested
            cerr << successors[i].streetName << " (" << successors[i].segment.start.latitudeText << ", " << successors[i].segment.start.longitudeText << ") " << " (" << successors[i].segment.end.latitudeText << ", " << successors[i].segment.end.longitudeText << ") " << endl;
            
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
                        untriedPath.push(*attractionNode);   // we'll try to visit it next time: it's pushed to the priority queue
                    }
                }
            }
            
            if (successors[i].segment.end == parentNode.getGeoCoord()) {
                sideToTest = successors[i].segment.start;
            }
            else if (successors[i].segment.start == parentNode.getGeoCoord()) {
                sideToTest = successors[i].segment.end;
            }
            else {
                sideToTest = successors[i].segment.start;
                
                NavNode* successor = new NavNode(sideToTest, parentNode.getLevel(), parentNode.getPriority(), parentNode.getPath());
                
                // otherwise
                // update the node's level and get its priority
                successor->nextLevel(&parentNode);
                successor->updatePriority(goal);

                // nodes visited
                double*  testing = nodesVisited.find(successor->getGeoCoord());  // TODO: may have to change the name of the variable
                if (testing == nullptr || (testing != nullptr && successor->getPriority() < *testing)) {   // it hasn't been visited, or it has been and the new priority is less than
                    untriedPath.push(*successor);   // we'll try to visit it next time: it's pushed to the priority queue
                }
                
                delete successor;   // TODO: pray that it pushes a copy of the node to the priority queue
                
                sideToTest = successors[i].segment.start;
                
                successor = new NavNode(sideToTest, parentNode.getLevel(), parentNode.getPriority(), parentNode.getPath());
                
                // otherwise
                // update the node's level and get its priority
                successor->nextLevel(&parentNode);
                successor->updatePriority(goal);
                
                // nodes visited
                testing = nodesVisited.find(successor->getGeoCoord());  // TODO: may have to change the name of the variable
                if (testing == nullptr || (testing != nullptr && successor->getPriority() < *testing)) {   // it hasn't been visited, or it has been and the new priority is less than something already visited
                    untriedPath.push(*successor);   // we'll try to visit it next time: it's pushed to the priority queue
                }
                
                delete successor;   // TODO: pray that it pushes a copy of the node to the priority queue
                
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
                untriedPath.push(*successor);   // we'll try to visit it next time: it's pushed to the priority queue
            }
            
            delete successor;   // TODO: pray that it pushes a copy of the node to the priority queue
        }
        
    }
    
    cerr << "Couldn't find " << endl;
    vector<GeoCoord> empty;
    return empty;
}


// whenever pushing to the queue, set visited to true


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
