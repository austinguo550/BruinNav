//
//  support.cpp
//  BruinNav
//
//  Created by Austin Guo on 3/13/17.
//  Copyright © 2017 Austin Guo. All rights reserved.
//

#include "support.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// TODO: OPERATORS REDEFINED
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


NavNode::NavNode(GeoCoord gc, int l, int p, std::vector<NavNode> prevPath) {
    currentPos = gc;
    level = l;
    priority = p;
    path = prevPath;
    path.push_back(*this);
}

std::vector<NavNode> NavNode::getPath() {
    return path;
}

GeoCoord NavNode::getGeoCoord() const {
    return currentPos;
}

double NavNode::getLevel() const {
    return level;
}

double NavNode::getPriority() const {
    return priority;
}

void NavNode::updatePriority(GeoCoord gcEnd) {
    priority = level + distanceEarthMiles(currentPos, gcEnd);
}

void NavNode::nextLevel(NavNode* previous) {
    if (previous == nullptr) {  // it's the first node, there's been no previous nodes before
        level = 0;
    }
    level += previous->getLevel() + distanceEarthMiles(previous->getGeoCoord(), currentPos);
}


bool operator<(const NavNode& a, const NavNode& b) {    // if the node is less than, then priority distance must be greater than (priority lower is better)
    return a.getPriority() > b.getPriority();
}

bool operator>(const NavNode& a, const NavNode& b) {
    return a.getPriority() < b.getPriority();
}

bool operator==(const NavNode& a, const NavNode& b) {
    return a.getPriority() == b.getPriority();
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
