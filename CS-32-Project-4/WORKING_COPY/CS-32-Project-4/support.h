//
//  support.hpp
//  CS-32-Project-4
//
//  Created by Edward Chu on 12/3/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"
#include <vector>
#include <string>

//Defines < 
class BetterGeoCoord : GeoCoord {
public:
    BetterGeoCoord(){
        latitude = 0;
        longitude = 0;
        latitudeText = "";
        longitudeText = "";
    }
    
    BetterGeoCoord(const GeoCoord& other){
        latitude = other.latitude;
        longitude = other.longitude;
        latitudeText = other.latitudeText;
        longitudeText = other.longitudeText;
    }
    
    bool operator <(const BetterGeoCoord& other) const;
    bool operator ==(const BetterGeoCoord& other) const;
    bool operator !=(const BetterGeoCoord& other) const;
};

class NaviNode {
public:
    NaviNode(NaviNode* parent, const GeoCoord& location, const GeoCoord& objective, StreetSegment& sg, bool isStartingNode = false){
        this->parent = parent;
        loc = location;
        streetSeg = sg;
        
        if(isStartingNode){
            f = h = distanceEarthMiles(location, objective);
            g = 0;
        } else {
            g = parent->g + distanceEarthMiles(parent->loc, location);
            h = distanceEarthMiles(location, objective);
            f = g + h;
        }
            
    }
    NaviNode* parent;
    GeoCoord loc;
    StreetSegment streetSeg;
    
    double f, g, h; //For A* Algorithm
    
    bool hasCoord(const GeoCoord& other) const;
    bool hasStreetSegment(const StreetSegment& other) const;
    bool operator <(const NaviNode& other) const;
private:
    
};

bool isSameGeoCoord(const GeoCoord& gc1, const GeoCoord& gc2);

std::string angleToDirection(const double travelAngle);
std::string segmentDirection(const GeoSegment& gs);
std::string turnDirection(const GeoCoord& p1, const GeoCoord& p2, const GeoCoord& p3);

void printNavSegment(const NavSegment& nav);
void printNaviNode(const NaviNode& nav);
void printGeoCoord(const GeoCoord& gc);
std::vector<NaviNode> getNeighborNodes(NaviNode* n, const SegmentMapper& sm, const GeoCoord& endCoord);
#endif /* support_h */
