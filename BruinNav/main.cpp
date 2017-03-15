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
//    // insert code here...
//    MapLoader ml;
//    ml.load("/Users/austinguo550/Documents/CS32 2017/BruinNav/BruinNav/mapdata.txt");
//    example(ml);
//    example2(ml);
    Navigator nav;
    nav.loadMapData("/Users/austinguo550/Documents/CS32 2017/BruinNav/BruinNav/mapdata.txt");
    vector<NavSegment> directions;
    nav.navigate("Harvard-Westlake Middle School", "GreyStone Mansion", directions);
    
    for (int i = 0; i < directions.size(); i++) {
        if (directions[i].m_command == NavSegment::TURN) {
            cerr << directions[i].m_direction<< endl;
        }
    }
    
    return 0;
}




//// This is the BruinNav main routine.  If the executable built from this file
//// and the other .cpp files you write is named BruinNav (or BruinNav.exe on
//// Windows), then you can run it with
////  ./BruinNav theMapDataFileName "Start Attraction" "End Attraction"
//// to get the turn-by-turn instructions a user wants to see, or
////  ./BruinNav theMapDataFileName "Start Attraction" "End Attraction" -raw
//// to see the sequence of NavSegments produced by Navigator::navigate()
//// (Under Windows, say "BruinNav" instead of "./BruinNav")
//// For example, with the mapdata.txt file we supplied you,
////  ./BruinNav mapdata.txt "Harvard-Westlake Middle School" "GreyStone Mansion"
//// should produce something like
////   Routing...
////   You are starting at: Harvard-Westlake Middle School
////   Proceed 0.47 miles southeast on Brooklawn Drive
////   Turn right onto Angelo Drive
////   Proceed 0.43 miles east on Angelo Drive
////   Turn right onto Benedict Canyon Drive
////   Proceed 0.13 miles southeast on Benedict Canyon Drive
////   Turn left onto Hartford Way
////   Proceed 0.13 miles east on Hartford Way
////   Turn left onto Lexington Road
////   Proceed 0.63 miles east on Lexington Road
////   Turn right onto Alpine Drive
////   Proceed 0.07 miles southeast on Alpine Drive
////   Turn left onto West Sunset Boulevard
////   Proceed 0.20 miles northeast on West Sunset Boulevard
////   Turn left onto Mountain Drive
////   Proceed 0.15 miles northeast on Mountain Drive
////   Turn left onto Schuyler Road
////   Proceed 0.19 miles north on Schuyler Road
////   Turn right onto Stonewood Drive
////   Proceed 0.15 miles northeast on Stonewood Drive
////   You have reached your destination: GreyStone Mansion
////   Total travel distance: 2.5 miles
//// and
////  ./BruinNav mapdata.txt "Harvard-Westlake Middle School" "GreyStone Mansion" -raw
//// might produce 133 lines starting
////   Start: Harvard-Westlake Middle School
////   End:   GreyStone Mansion
////   34.0904161,-118.4344198 34.0905309,-118.4343340 northeast 0.0093 Brooklawn Drive
////   34.0905309,-118.4343340 34.0904815,-118.4341398 east 0.0116 Brooklawn Drive
////   34.0904815,-118.4341398 34.0903824,-118.4339467 southeast 0.0130 Brooklawn Drive
////   34.0903824,-118.4339467 34.0902310,-118.4337702 southeast 0.0145 Brooklawn Drive
////   34.0902310,-118.4337702 34.0900681,-118.4335630 southeast 0.0163 Brooklawn Drive
////   34.0900681,-118.4335630 34.0899633,-118.4334635 southeast 0.0092 Brooklawn Drive
////   34.0899633,-118.4334635 34.0897917,-118.4333739 southeast 0.0129 Brooklawn Drive
////   34.0897917,-118.4333739 34.0894654,-118.4333087 south 0.0229 Brooklawn Drive
//// and ending
////   34.0904163,-118.4036377 34.0905573,-118.4036590 north 0.0098 Schuyler Road
////   34.0905573,-118.4036590 34.0908428,-118.4038080 northwest 0.0215 Schuyler Road
////   turn right Stonewood Drive
////   34.0908428,-118.4038080 34.0908832,-118.4036471 east 0.0096 Stonewood Drive
////   34.0908832,-118.4036471 34.0908732,-118.4034846 east 0.0093 Stonewood Drive
////   34.0908732,-118.4034846 34.0908807,-118.4033732 east 0.0064 Stonewood Drive
////   34.0908807,-118.4033732 34.0909505,-118.4031144 east 0.0156 Stonewood Drive
////   34.0909505,-118.4031144 34.0909630,-118.4030512 east 0.0037 Stonewood Drive
////   34.0909630,-118.4030512 34.0909256,-118.4029338 east 0.0072 Stonewood Drive
////   34.0909256,-118.4029338 34.0919749,-118.4018226 northeast 0.0964 Stonewood Drive
////
//// If you build the program as is, you'll notice the turn-by-turn instructions
//// say IN_SOME_DIRECTION instead of east or southwest or some actual direction.
//// That's because of the template appearing a few lines below; read the comment
//// before it.
//
//#include "provided.h"
////#include "support.h"
//#include <iostream>
//#include <string>
//#include <vector>
//#include <cstring>
//#include "support.h"
//using namespace std;
//
//// START OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)
//// If you want the turn-by-turn directions to give a real direction like
//// east or southwest instead of IN_SOME_DIRECTION, you'll need to
//// implement the ordinary function
////    string directionOfLine(const GeoSegment& gs)
//// to return a string like "east" or "southwest" based on the angle of the
//// GeoSegment gs according to the table at the bottom of page 20 of the spec.
//// When you do that, you can delete this comment and the template function
//// below that appears here solely to allow this main.cpp to build.
//// Why didn't we just write the real function for you?  Because it's also
//// a function you'd find useful in producing the NavSegments in the navigate()
//// method.  Since it's useful in more than one .cpp file, its declaration
//// should go in support.h and its implementation in support.cpp.
//
////template<typename T>
////string directionOfLine(const T&)
////{
////    return "IN_SOME_DIRECTION";
////}
//// END OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)
//
//void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);
//void printDirections(string start, string end, vector<NavSegment>& navSegments);
//
//int main(int argc, char *argv[])
//{
//    bool raw = false;
//    if (argc == 5  &&  strcmp(argv[4], "-raw") == 0)
//    {
//        raw = true;
//        argc--;
//    }
//    if (argc != 4)
//    {
//        cout << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\"" << endl
//        << "or" << endl
//        << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\" -raw" << endl;
//        return 1;
//    }
//    
//    Navigator nav;
//    
//    if ( ! nav.loadMapData(argv[1]))
//    {
//        cout << "Map data file was not found or has bad format: " << argv[1] << endl;
//        return 1;
//    }
//    
//    if ( ! raw)
//        cout << "Routing..." << flush;
//    
//    string start = argv[2];
//    string end = argv[3];
//    vector<NavSegment> navSegments;
//    
//    NavResult result = nav.navigate(start, end, navSegments);
//    if ( ! raw)
//        cout << endl;
//    
//    switch (result)
//    {
//        case NAV_NO_ROUTE:
//            cout << "No route found between " << start << " and " << end << endl;
//            break;
//        case NAV_BAD_SOURCE:
//            cout << "Start attraction not found: " << start << endl;
//            break;
//        case NAV_BAD_DESTINATION:
//            cout << "End attraction not found: " << end << endl;
//            break;
//        case NAV_SUCCESS:
//            if (raw)
//                printDirectionsRaw(start, end, navSegments);
//            else
//                printDirections(start, end, navSegments);
//            break;
//    }
//}
//
//void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)
//{
//    cout << "Start: " << start << endl;
//    cout << "End:   " << end << endl;
//    cout.setf(ios::fixed);
//    cout.precision(4);
//    for (auto ns : navSegments)
//    {
//        switch (ns.m_command)
//        {
//            case NavSegment::PROCEED:
//                cout << ns.m_geoSegment.start.latitudeText << ","
//                << ns.m_geoSegment.start.longitudeText << " "
//                << ns.m_geoSegment.end.latitudeText << ","
//                << ns.m_geoSegment.end.longitudeText << " "
//                << ns.m_direction << " "
//                << ns.m_distance << " "
//                << ns.m_streetName << endl;
//                break;
//            case NavSegment::TURN:
//                cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
//                break;
//        }
//    }
//}
//
//void printDirections(string start, string end, vector<NavSegment>& navSegments)
//{
//    cout.setf(ios::fixed);
//    cout.precision(2);
//    
//    cout << "You are starting at: " << start << endl;
//    
//    double totalDistance = 0;
//    string thisStreet;
//    GeoSegment effectiveSegment;
//    double distSinceLastTurn = 0;
//    
//    for (auto ns : navSegments)
//    {
//        switch (ns.m_command)
//        {
//            case NavSegment::PROCEED:
//                if (thisStreet.empty())
//                {
//                    thisStreet = ns.m_streetName;
//                    effectiveSegment.start = ns.m_geoSegment.start;
//                }
//                effectiveSegment.end = ns.m_geoSegment.end;
//                distSinceLastTurn += ns.m_distance;
//                totalDistance += ns.m_distance;
//                break;
//            case NavSegment::TURN:
//                if (distSinceLastTurn > 0)
//                {
//                    cout << "Proceed " << distSinceLastTurn << " miles "
//                    << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
//                    thisStreet.clear();
//                    distSinceLastTurn = 0;
//                }
//                cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
//                break;
//        }
//    }
//    
//    if (distSinceLastTurn > 0)
//        cout << "Proceed " << distSinceLastTurn << " miles "
//        << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
//    cout << "You have reached your destination: " << end << endl;
//    cout.precision(1);
//    cout << "Total travel distance: " << totalDistance << " miles" << endl;
//}
