//
//  support.cpp
//  CS-32-Project-4
//
//  Created by Edward Chu on 12/3/2017.
//  Copyright © 2017 Puffins. All rights reserved.
//

#include "support.h"
#include <iostream>

bool BetterGeoCoord::operator < (const BetterGeoCoord& other) const{
    if(latitude < other.latitude) return true;
    else if(other.latitude < latitude) return false;
    
    if(longitude < other.longitude) return true;
    else return false;
}

bool BetterGeoCoord::operator ==(const BetterGeoCoord& other) const{
    if(latitude == other.latitude && longitude == other.longitude){
        if(latitudeText == other.latitudeText && longitudeText == other.longitudeText)
            return true;
    }
    
    return false;
}

bool BetterGeoCoord::operator !=(const BetterGeoCoord& other) const{
    return !(*this == other);
}

bool NaviNode::hasCoord(const GeoCoord& other) const{
    //BetterGeoCoord here = BetterGeoCoord(this->loc);
    //BetterGeoCoord there = BetterGeoCoord(other);
    
    if(other.latitudeText == loc.latitudeText && other.longitudeText == loc.longitudeText)
        return true;
    
    return false;
    
    //return here == there;
}

bool NaviNode::hasStreetSegment(const StreetSegment& other) const{
    if(BetterGeoCoord(streetSeg.segment.start) == BetterGeoCoord(other.segment.start)){
        if(BetterGeoCoord(streetSeg.segment.end) == BetterGeoCoord(other.segment.end))
            return true;
    }
    
    return false;
}

bool NaviNode::operator <(const NaviNode& other) const {
    if(this->f < other.f) return true;
    else return false;
}

void printGeoCoord(const GeoCoord& gc){
    std::cerr << "(" << gc.latitudeText << ", " << gc.longitudeText << ")" << std::endl;
}

void printNaviNode(const NaviNode& nav){
    std::cerr << "Printing navi node" << std::endl;
    std::cerr << "Street Name: " << nav.streetSeg.streetName << std::endl;
    printGeoCoord(nav.loc);
    std::cerr << "Street start: ";
    printGeoCoord(nav.streetSeg.segment.start);
    std::cerr << "Street end: ";
    printGeoCoord(nav.streetSeg.segment.end);
    
    if(nav.parent != nullptr){
        std::cerr << "Parent Street: " << nav.parent->streetSeg.streetName << std::endl;
        printGeoCoord(nav.parent->loc);
    }
    
}

std::string angleToDirection(const double travelAngle){
    if(0 <= travelAngle && travelAngle <= 22.5)
        return "east";
    else if(travelAngle <= 62.5)
        return "northeast";
    else if(travelAngle <= 112.5)
        return "north";
    else if(travelAngle <= 157.5)
        return "northwest";
    else if(travelAngle <= 202.5)
        return "west";
    else if(travelAngle <= 247.5)
        return "southwest";
    else if(travelAngle <= 292.5)
        return "south";
    else if(travelAngle <= 337.5)
        return "southeast";
    else if(travelAngle < 360)
        return "east";
    
    return "invalid angle";
}

std::string segmentDirection(const StreetSegment& ss){
    return angleToDirection(angleOfLine(ss.segment));
}

std::string turnDirection(const GeoCoord& p1, const GeoCoord& p2, const GeoCoord& p3){
    //Cross product
    double v1x = p2.latitude - p1.latitude;
    double v1y = p2.longitude - p1.longitude;
    double v2x = p3.latitude - p2.latitude;
    double v2y = p3.longitude - p2.longitude;
    
    if(v1x * v2y - v1y * v2x < 0.0)
        return "left";
    else
        return "right";
}

void printNavSegment(const NavSegment& nav){
    std::cerr << std::endl;
    if(nav.m_command == 0)
        std::cerr << "type: PROCEED" << std::endl;
    else
        std::cerr << "type: TURN" << std::endl;
    
    std::cerr << "start: ";
    printGeoCoord(nav.m_geoSegment.start);
    
    std::cerr << "end: ";
    printGeoCoord(nav.m_geoSegment.end);
    
    std::cerr << "Direction: " << nav.m_direction << std::endl;
    std::cerr << "Distance: " << nav.m_distance << std::endl;
    std::cerr << "Street: " << nav.m_streetName << std::endl;
    std::cerr << std::endl;
}
