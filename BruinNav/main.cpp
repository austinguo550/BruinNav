//
//  main.cpp
//  BruinNav
//
//  Created by Austin Guo on 3/8/17.
//  Copyright © 2017 Austin Guo. All rights reserved.
//

#include <iostream>
#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

void example(const MapLoader& ml) {
    AttractionMapper am;
    am.init(ml); // let our object build its internal data structures
    // by iterating thru all segments from the MapLoader object
    GeoCoord fillMe;
    string attraction = "The Coffee Bean & Tea Leaf";
    bool found = am.getGeoCoord(attraction, fillMe); if ( ! found)
    {
        cout << "No geolocation found for " << attraction << endl;
        return; }
    cout << "The location of " << attraction << " is " << fillMe.latitude << ", " << fillMe.longitude << endl;
}

void foo() {
    MyMap<string,double> nameToGPA; // maps student name to GPA
    // add new items to the binary search tree-based map
    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
    nameToGPA.associate("David", 3.99); // David beat Carey
    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr)
        *davidsGPA = 1.5; // after a re-grade of David’s exam
    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0 // replaces old 3.5 GPA
    nameToGPA.associate("Linda", 6.8);
    double* lindasGPA = nameToGPA.find("Carey");
    if (lindasGPA == nullptr)
        cout << "Linda is not in the roster!" << endl;
    else cout << "Carey's GPA is: " << *lindasGPA << endl;
}

// segment mapper
void example2(const MapLoader& ml) {
    SegmentMapper sm;
    sm.init(ml); // let our object build its internal data structures
    // by iterating thru all segments from the MapLoader object
    GeoCoord lookMeUp("34.0572000", "-118.4417620");
    std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
    if (vecOfAssociatedSegs.empty())
    {
        cout << "Error - no segments found matching this coordinate\n";
        return;
    }
    cout << "Here are all the segments associated with your coordinate:" << endl;
    for (auto s: vecOfAssociatedSegs) {
        cout << "Segment’s street: " << s.streetName << endl;
        cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " <<
        s.segment.start.longitude << endl; cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " <<
        s.segment.end.longitude << endl; cout << "This segment has " << s.attractions.size() <<
        " attractions on it." << endl;
    }
}


int main(int argc, const char * argv[]) {
//    foo();
    // insert code here...
//    MapLoader ml;
//    ml.load("/Users/austinguo550/Documents/CS32 2017/BruinNav/BruinNav/mapdata.txt");
//    example(ml);
//    example2(ml);
    Navigator nav;
    nav.loadMapData("/Users/austinguo550/Documents/CS32 2017/BruinNav/BruinNav/mapdata.txt");
    vector<NavSegment> directions;
    nav.navigate("Fox Plaza", "Century City Heliport", directions);
    
    return 0;
}
