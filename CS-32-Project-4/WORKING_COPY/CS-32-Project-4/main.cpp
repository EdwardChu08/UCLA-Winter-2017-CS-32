// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include "support.h"


#include <string>
#include <iostream>
#include <cassert>

using namespace std;

void foo()
{
    MyMap<std::string,double> nameToGPA; // maps student name to GPA
    // add new items to the binary search tree-based map
    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
    nameToGPA.associate("David", 3.99); // David beat Carey
    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr)
        *davidsGPA = 1.5; // after a re-grade of David’s exam
    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
    // replaces old 3.5 GPA
    double* lindasGPA = nameToGPA.find("Linda");
    
    if (lindasGPA == nullptr)
        cerr << "Linda is not in the roster!" << endl;
    else
        cerr << "Linda’s GPA is: " << *lindasGPA << endl;
    
    cerr << nameToGPA.size() << endl;
    
}



int main()
{
    const std::string MAP_DATA = "/Users/edwardchu/Desktop/CS-32/CS-32-Project-4/CS-32-Project-4/CS-32-Project-4/mapdata.txt";
    
    foo();
    
    MapLoader ml;
    
    ml.load(MAP_DATA);
    
    StreetSegment ss;
    ml.getSegment(2, ss);
    cerr << ss.streetName << endl;
    cerr << ss.segment.start.latitudeText << ", " << ss.segment.start.longitudeText << endl;
    
    AttractionMapper am;
    am.init(ml);
    
    GeoCoord fillMe;
    assert(am.getGeoCoord("ThAlians MentAl HeaLth CenTer", fillMe));
    assert(fillMe.latitudeText == "34.0751447" && fillMe.longitudeText == "-118.3830838");
    
    assert(!am.getGeoCoord("Not exist", fillMe));
    assert(fillMe.latitudeText == "34.0751447" && fillMe.longitudeText == "-118.3830838");
    
    SegmentMapper sm;
    sm.init(ml);
    
    vector<StreetSegment> vs;
    vs = sm.getSegments(fillMe);
    for(int i = 0; i < vs.size(); i++){
        cerr << vs[i].streetName << endl;
    }
    
    am.getGeoCoord("Veterans Administration Medical Center West Los Angeles Heliport", fillMe);
    vs = sm.getSegments(fillMe);
    for(int i = 0; i < vs.size(); i++){
        cerr << vs[i].streetName << endl;
    }
    GeoCoord g1("34.0613269", "-118.4462765");
    GeoCoord g2("34.0613323", "-118.4461140");
    cerr << angleOfLine(GeoSegment(g1, g2)) << endl;
    
    Navigator nav;
    nav.loadMapData(MAP_DATA);
    std::vector<NavSegment> directions;
    nav.navigate("1061 Broxton Avenue", "Headlines!", directions);
    //nav.navigate("1031 Broxton Avenue", "The Maltz Park", directions);
    //nav.navigate("Ami Sushi", "The Annenberg Space for Photography", directions);
    //nav.navigate("Whisky a Go Go", "Flame International", directions);
    //nav.navigate("1061 Broxton Avenue", "Botany Building", directions);
    
    cerr << "All tests succeeded" << endl;
}

