#include "provided.h"
#include "MyMap.h"

#include <string>
#include <algorithm>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
    
private:
    MyMap<string, GeoCoord> m_attractionCoords;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++){
        StreetSegment sg;
        ml.getSegment(i, sg);
        
        for(int j = 0; j < sg.attractions.size(); j++){
            //Turn attraction name to lowercase
            string attractionName = sg.attractions[j].name;
            transform(attractionName.begin(), attractionName.end(), attractionName.begin(), ::tolower);
            
            m_attractionCoords.associate(attractionName, sg.attractions[j].geocoordinates);
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	transform(attraction.begin(), attraction.end(), attraction.begin(), ::tolower);
    const GeoCoord* loc = m_attractionCoords.find(attraction);
    
    if(loc == nullptr) return false; //Not found
    
    //Found!
    gc = *loc;
    return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
