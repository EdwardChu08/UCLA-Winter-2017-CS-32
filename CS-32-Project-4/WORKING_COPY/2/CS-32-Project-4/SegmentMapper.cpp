#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>

using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
private:
    MyMap<BetterGeoCoord, vector<StreetSegment>> m_coordsToSegment;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++){
        StreetSegment sg;
        ml.getSegment(i, sg);
        
        vector<StreetSegment>* get;
        
        //Start
        get = m_coordsToSegment.find(BetterGeoCoord(sg.segment.start));
        if(get == nullptr)
            m_coordsToSegment.associate(BetterGeoCoord(sg.segment.start), vector<StreetSegment>({sg}));
        else
            get->push_back(sg);
        
        
        //End
        get = m_coordsToSegment.find(BetterGeoCoord(sg.segment.end));
        if(get == nullptr)
            m_coordsToSegment.associate(BetterGeoCoord(sg.segment.end), vector<StreetSegment>({sg}));
        else
            get->push_back(sg);
        
        //Mapping Attractions geoCoords to their StreetSegment
        for(int j = 0; j < sg.attractions.size(); j++){
            get = m_coordsToSegment.find(BetterGeoCoord(sg.attractions[j].geocoordinates));
            if(get == nullptr)
                m_coordsToSegment.associate(BetterGeoCoord(sg.attractions[j].geocoordinates), vector<StreetSegment>({sg}));
            else
                get->push_back(sg);
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	return *m_coordsToSegment.find(BetterGeoCoord(gc));  // This compiles, but may not be correct
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
