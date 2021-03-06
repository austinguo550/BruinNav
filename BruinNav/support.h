//
//  overloaded.h
//  BruinNav
//
//  Created by Austin Guo on 3/12/17.
//  Copyright © 2017 Austin Guo. All rights reserved.
//

#ifndef overloaded_h
#define overloaded_h

#include "provided.h"
#include <vector>

bool operator>(const GeoCoord &a, const GeoCoord &b);
bool operator<(const GeoCoord &a, const GeoCoord &b);
bool operator==(const GeoCoord &a, const GeoCoord &b);

class NavNode {
    //current position
    GeoCoord currentPos;
    double level;  // cost g
    // priority = `g + h (level + remaining distance estimate)
    double priority;   // smaller: higher priority
    std::vector<GeoCoord> path;
    
public:
    NavNode(GeoCoord gc, int l, int p, std::vector<GeoCoord> prevPath);
    
    std::vector<GeoCoord> getPath();
    
    GeoCoord getGeoCoord() const;
    
    double getLevel() const;
    
    double getPriority() const;
    
    void updatePriority(GeoCoord gcEnd);
    
    void nextLevel(NavNode* previous);
    
    
};

bool operator<(const NavNode& a, const NavNode& b);


bool operator>(const StreetSegment &a, const StreetSegment &b);

bool operator<(const StreetSegment &a, const StreetSegment &b);

bool operator==(const StreetSegment &a, const StreetSegment &b);


std::string directionOfLine(const GeoSegment& gs);


#endif /* overloaded_h */
