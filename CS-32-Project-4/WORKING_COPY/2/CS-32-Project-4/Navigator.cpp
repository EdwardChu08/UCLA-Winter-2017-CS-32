#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <iostream>
using namespace std;

std::vector<NaviNode> getNeighborNodes(NaviNode* n, const SegmentMapper& sm, const GeoCoord& endCoord);
vector<NavSegment> getReturnRoute(NaviNode endNode);

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
    
private:
    MapLoader ml;
    AttractionMapper am;
    SegmentMapper sm;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if(!ml.load(mapFile)){
        return false;
    }
    
    am.init(ml);
    sm.init(ml);
    return true;
}


NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    GeoCoord startCoord;
    if(!am.getGeoCoord(start, startCoord))
        return NAV_BAD_SOURCE;
    
    GeoCoord endCoord;
    if(!am.getGeoCoord(end, endCoord))
        return NAV_BAD_DESTINATION;
    
    std::cerr << "-----------------------" << std::endl;
    std::cerr << "From: " << start << std::endl;
    std::cerr << "To: " << end << std::endl << std::endl;
    std::cerr << "Navigation starting... " << std::endl;
    
    //A* algorithm
    //Here we go!
    
    list<NaviNode*> openList; //Should always be sorted by f value, acts like a priority queue
    list<NaviNode*> closedList;
    
    StreetSegment startStreet = sm.getSegments(startCoord)[0];
    NaviNode* startNode = new NaviNode(nullptr, startCoord, endCoord, startStreet, true);
    openList.push_back(startNode);
    
    
    while(openList.size() > 0){
        NaviNode* current = openList.front();
        openList.pop_front();
        
        vector<NaviNode> neighbors = getNeighborNodes(current, sm, endCoord);
        for(NaviNode neighbor : neighbors){
            
            if(neighbor.hasCoord(endCoord)){
                //Found result!
                directions = getReturnRoute(neighbor);
                
                //Clean up
                delete current;
                for(NaviNode* node : openList)
                    delete node;
                
                for(NaviNode* node : closedList)
                    delete node;
                
                return NAV_SUCCESS;
            }
            
            bool putIn = true;
            //Check if node already exist on open/closed list with a lower f value
            
            for(NaviNode* node : openList){
                if(node->hasCoord(neighbor.loc) && node->f < neighbor.f){
                    putIn = false;
                    break;
                }
            }
            
            for(NaviNode* node : closedList){
                if(putIn && node->hasCoord(neighbor.loc) && node->f < neighbor.f){
                    putIn = false;
                    break;
                }
            }
            
            //Otherwise, insert into correct pos in openList s.t. openList is sorted by f value
            if(putIn){
                NaviNode* successor = new NaviNode(neighbor);
                
                if(openList.empty()){
                    openList.push_back(successor);
                }
                else if(openList.back()->f <= successor->f)
                    openList.push_back(successor);
                else {
                    //Insert into position s.t. openList is sorted by f
                    for(auto it = openList.begin(); it != openList.end(); it++){
                        if(successor->f < (*it)->f){
                            openList.insert(it, successor);
                            break;
                        }
                    }
                }
            }
        }
        
        closedList.push_back(current);
    }
    
    //Clean up
    for(NaviNode* node : openList){
        delete node;
    }
    
    for(NaviNode* node : closedList){
        delete node;
    }

    std::cerr << "No route to Destination." << std::endl;
    std::cerr << "-----------------------" << std::endl;
	return NAV_NO_ROUTE;
}

std::vector<NaviNode> getNeighborNodes(NaviNode* n, const SegmentMapper& sm, const GeoCoord& endCoord){
    std::vector<NaviNode> neighbors;
    
    //Getting nodes on neighboring streets
    std::vector<StreetSegment> connectingStreets = sm.getSegments(n->loc);
    for(StreetSegment connectingStreet : connectingStreets){
        
        //Check if n is the same as the start/end of a segment, do not add if true
        if(!n->hasCoord(connectingStreet.segment.start))
            neighbors.push_back(NaviNode(n, connectingStreet.segment.start, endCoord, connectingStreet));
        
        if(!n->hasCoord(connectingStreet.segment.end))
            neighbors.push_back(NaviNode(n, connectingStreet.segment.end, endCoord, connectingStreet));
        
        //Getting attractions
        for(Attraction a : connectingStreet.attractions){
            //n is this attractions!
            if(n->hasCoord(a.geocoordinates))
                continue;
            
            neighbors.push_back(NaviNode(n, a.geocoordinates, endCoord, connectingStreet));
        }
    }
    
    return neighbors;
}


vector<NavSegment> getReturnRoute(NaviNode endNode){
    stack<NavSegment> navStack;
    NaviNode* currNode = &endNode;
    
    while (currNode->parent != nullptr) {
        std::string direction = segmentDirection(currNode->streetSeg);
        double distance = distanceEarthMiles(currNode->loc, currNode->parent->loc);
        std::string streetName = currNode->streetSeg.streetName;
        GeoSegment seg(currNode->parent->loc, currNode->loc);
        
        navStack.push(NavSegment(direction, streetName, distance, seg));
        
        //Turning onto new street
        if(currNode->streetSeg.streetName != currNode->parent->streetSeg.streetName){
            //This is okay because a turn segment always have at least 3 points
            GeoCoord p1 = currNode->parent->parent->loc;
            GeoCoord p2 = currNode->parent->loc;
            GeoCoord p3 = currNode->loc;
            std::string turnDir = turnDirection(p1, p2, p3);
            navStack.push(NavSegment(turnDir, currNode->streetSeg.streetName));
        }
        
        currNode = currNode->parent;
    }
    
    vector<NavSegment> route;
    while(!navStack.empty()){
        route.push_back(navStack.top());
        navStack.pop();
    }
    
    
    for(int i = 0; i < route.size(); i++){
        printNavSegment(route[i]);
    }
    
    std::cerr << "Route completed!" << std::endl;
    std::cerr << "-----------------------" << std::endl;
    std::cerr << std::endl;
    return route;
}



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
