//
//  support.cpp
//  BruinNav
//
//  Created by Austin Guo on 3/13/17.
//  Copyright © 2017 Austin Guo. All rights reserved.
//

#include "support.h"
#include <cmath>
#include "provided.h"
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////
// TODO: CHANGE BACK TO LATITUDE/LONGITUDE instead of LATITUDE TEXT AND LONGITUDE TEXT???
//
bool operator>(const GeoCoord &a, const GeoCoord &b) {
    if (a.latitude > b.latitude) {
        return true;
    }
    else if (a.latitude == b.latitude) {
        if (a.longitude > b.longitude) {
            return true;
        }
    }
    return false;
}

bool operator<(const GeoCoord &a, const GeoCoord &b) {
    if (a.latitude < b.latitude) {
        return true;
    }
    else if (a.latitude == b.latitude) {
        if (a.longitude < b.longitude) {
            return true;
        }
    }
    return false;
}

bool operator==(const GeoCoord &a, const GeoCoord &b) {
    if (a.latitude == b.latitude) {
        if (a.longitude == b.longitude) {
            return true;
        }
    }
    return false;
}


NavNode::NavNode(GeoCoord gc, int l, int p, std::vector<GeoCoord> prevPath) {
    currentPos = gc;
    level = l;
    priority = p;
    if (prevPath.size() > 0) path = prevPath;
    path.push_back(gc);
}

std::vector<GeoCoord> NavNode::getPath() {
    return path;
}

GeoCoord NavNode::getGeoCoord() const {
    return currentPos;
}

double NavNode::getLevel() const {
    return level;   // g
}

double NavNode::getPriority() const {
    return priority;
}

void NavNode::updatePriority(GeoCoord gcEnd) {
    priority = level + fabs(distanceEarthMiles(currentPos, gcEnd));   // f = g + h
}

void NavNode::nextLevel(NavNode* previous) {
    if (previous == nullptr) {  // it's the first node, there's been no previous nodes before
        level = 0;
    }
    level = previous->getLevel() + fabs(distanceEarthMiles(previous->getGeoCoord(), currentPos));
}


bool operator<(const NavNode& a, const NavNode& b) {    // if the node is less than, then priority distance must be greater than (priority lower is better)
    return a.getPriority() > b.getPriority();
}



bool operator>(const StreetSegment &a, const StreetSegment &b) {
    //    if (a.streetName > b.streetName) {
    //        return true;
    //    }
    //else if (a.streetName == b.streetName) {
    if (a.segment.start > b.segment.start) {
        return true;
    }
    else if (a.segment.start == b.segment.start) {
        if (a.segment.end > b.segment.end) {
            return true;
        }
        // otherwise they are the same
    }
    //}
    return false;
}

bool operator<(const StreetSegment &a, const StreetSegment &b) {
    //    if (a.streetName < b.streetName) {
    //        return true;
    //    }
    //else if (a.streetName == b.streetName) {
    if (a.segment.start < b.segment.start) {
        return true;
    }
    else if (a.segment.start == b.segment.start) {
        if (a.segment.end < b.segment.end) {
            return true;
        }
        // otherwise they are the same
    }
    //}
    return false;
}

bool operator==(const StreetSegment &a, const StreetSegment &b) {
    //if (a.streetName == b.streetName) {
    if (a.segment.start == b.segment.start) {
        if (a.segment.end == b.segment.end) {
            return true;
        }
    }
    //}
    return false;
}



/*
 0 degrees <= travelAngle <= 22.5 degrees: east
 ● 22.5 degrees < travelAngle <= 67.5 degrees: northeast
 ● 67.5 degrees < travelAngle <= 112.5 degrees: north
 ● 112.5 degrees < travelAngle <= 157.5 degrees: northwest
 ● 157.5 degrees < travelAngle <= 202.5 degrees: west
 ● 202.5 degrees < travelAngle <= 247.5 degrees: southwest
 ● 247.5 degrees < travelAngle <= 292.5 degrees: south
 ● 292.5 degrees < travelAngle <= 337.5 degrees: southeast
 ● 337.5 degrees < travelAngle < 360 degrees: east
 */
string directionOfLine(const GeoSegment& gs) {
    double angle = angleOfLine(gs);
    
    if (0 <= angle && angle <= 22.5) {
        return "east";
    }
    else if (22.5 < angle && angle <= 67.5) {
        return "northeast";
    }
    else if (67.5 < angle && angle <= 112.5) {
        return "north";
    }
    else if (112.5 < angle && angle <= 157.5) {
        return "northwest";
    }
    else if (157.5 < angle && angle <= 202.5) {
        return "west";
    }
    else if (202.5 < angle && angle <= 247.5) {
        return "southwest";
    }
    else if (247.5 < angle && angle <= 292.5) {
        return "south";
    }
    else if (292.5 < angle && angle <= 337.5) {
        return "southeast";
    }
    else if (337.5 < angle && angle < 360) {
        return "east";
    }
    
    // otherwise the angle doesn't exist
    return "direction doesn't exist/is invalid";
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
