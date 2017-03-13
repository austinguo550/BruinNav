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
    
    vector<NavNode> pathFind(string begin, string destination) const;
};



NavigatorImpl::NavigatorImpl()
{

}

NavigatorImpl::~NavigatorImpl()
{
}

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
    
    vector<NavNode> pathToDisplay = pathFind(start, end);
    if (pathToDisplay.empty()) {
        return NAV_NO_ROUTE;  // This compiles, but may not be correct
    }
    
    // turn directions to pathToDisplay, except pathToDisplay needs to use navSegments
    return NAV_SUCCESS;
	
}



vector<NavNode> NavigatorImpl::pathFind(string begin, string destination) const {
    
    // write helper function to pass in GeoCoords
    GeoCoord start, goal;
    attractionMapper.getGeoCoord(begin, start);
    attractionMapper.getGeoCoord(destination, goal);
    
    
    priority_queue<NavNode> untriedPath;
    //    int pqI;
    NavNode* startNode;
    //    Node* endNode;
    
    vector<NavNode> pathBeforeStart;   // before starting, the path is empty
    startNode = new NavNode(start, 0, 0, pathBeforeStart);
    //    endNode = new Node(start, 0, 0, pathBeforeStart);
    untriedPath.push(*startNode);
    
    MyMap<GeoCoord, double> openNodes;
    MyMap<GeoCoord, double> nodesVisited;
    
    while (!untriedPath.empty())    // there's still something in the queue
    {
        NavNode parentNode = untriedPath.top();
        untriedPath.pop();
        if (openNodes.find(parentNode.getGeoCoord()) != nullptr) {
            openNodes.associate(parentNode.getGeoCoord(), -1);
        }
        // TODO check to see if parent is end
        
        vector<StreetSegment> successors;
        successors = segMap.getSegments(parentNode.getGeoCoord());
        
        // have a bunch of street segments
        for (int i = 0; i < successors.size(); i++) {
            // the successive segments produced could have either their end or start connected to the parent node
            GeoCoord sideToTest;
            int numberOfSidesToTest = 1;
            if (successors[i].segment.start == parentNode.getGeoCoord()) {
                sideToTest = successors[i].segment.end;
            }
            else if (successors[i].segment.end == parentNode.getGeoCoord()) {
                sideToTest = successors[i].segment.start;
            }
            else {  // it's associated by the attractions
                sideToTest = successors[i].segment.start;
                numberOfSidesToTest = 2;
            }
            
            for (int j = 0; j < numberOfSidesToTest; j++) { // if it's associated by the attractions, it will run twice
                if (numberOfSidesToTest == 2) {
                    sideToTest = successors[i].segment.end;
                }
                
                if (nodesVisited.find(sideToTest) == nullptr) { // if the node hasn't been visited yet
                    
                    NavNode* successor = new NavNode(sideToTest, parentNode.getLevel(), parentNode.getPriority(), parentNode.getPath());
                    if (sideToTest == goal) {   // we've found the right path!
                        nodesVisited.clear();   // about to return, must clear the mymap to avoid memory leaks
                        return successor->getPath();
                    }
                    
                    // otherwise
                    // update the node's level and get its priority
                    successor->nextLevel(&parentNode);
                    successor->updatePriority(goal);
                    
                    
                    bool shouldPushToOpen = true;
                    // open nodes
                    if (openNodes.find(successor->getGeoCoord()) != nullptr) {
                        if ((*openNodes.find(successor->getGeoCoord())) != -1) {
                            double f = *(nodesVisited.find(successor->getGeoCoord()));
                            if (successor->getPriority() > f) {
                                shouldPushToOpen = false;
                            }
                        }
                    }
                    
                    // nodes visited
                    if (nodesVisited.find(successor->getGeoCoord()) != nullptr) {   // it has been visited
                        double f = *(nodesVisited.find(successor->getGeoCoord()));
                        if (successor->getPriority() > f) {
                            shouldPushToOpen = false;
                        }
                    }
                    
                    if (shouldPushToOpen) {
                        untriedPath.push(*successor);   // we'll try to visit it next time: it's pushed to the priority queue
                        openNodes.associate(successor->getGeoCoord(), successor->getPriority());
                    }
                    
                    delete successor;   // TODO: pray that it pushes a copy of the node to the priority queue
                }
            }
        }
        
        nodesVisited.associate(parentNode.getGeoCoord(), parentNode.getPriority());        // the node has been visited: no two nodes can have the same geocoord
    }
    
    nodesVisited.clear();   // about to return, must clear the mymap to avoid memory leaks
    vector<NavNode> empty;
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
