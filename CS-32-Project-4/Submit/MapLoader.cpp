#include "provided.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    
private:
    size_t m_size;
    vector<StreetSegment*> m_segments;
};

MapLoaderImpl::MapLoaderImpl() : m_size(0)
{
}

MapLoaderImpl::~MapLoaderImpl()
{
    for(int i = 0; i < m_size; i++){
        delete m_segments[i];
    }
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if (!infile)		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open map data!" << endl;
        return false;
    }
    
    string s;
    while (getline(infile, s))
    {
        //Getting name of segment
        m_segments.push_back(new StreetSegment);
        m_segments[m_size]->streetName = s;
        
        //Getting start and end coords of segment
        string coords[4];
        getline(infile, s, ',');
        coords[0] = s; //First coord
        
        getline(infile, s, ' ');//Ignore first space
        if(s == "")
            getline(infile, s, ' ');
        coords[1] = s; //Second coord
        
        getline(infile, s, ',');
        coords[2] = s; //Third coord
        
        getline(infile, s);
        coords[3] = s; //Fourth coord
        
        GeoCoord startCoord(coords[0], coords[1]);
        GeoCoord endCoord(coords[2], coords[3]);
        m_segments[m_size]->segment = GeoSegment(startCoord, endCoord);
        
        //Getting attractions
        getline(infile, s);
        int numAttractions = stoi(s);
        
        for(int i = 0; i < numAttractions; i++){
            getline(infile, s, '|');
            string attractionName = s;
            
            getline(infile, s, ',');
            string lat = s;
            getline(infile, s);
            if(s[0] == ' ')
                s = s.substr(1);
            string lon = s;
            GeoCoord loc(lat, lon);
            
            Attraction a;
            a.name = attractionName;
            a.geocoordinates = loc;
            
            m_segments[m_size]->attractions.push_back(a);
        }
        
        m_size++;
    }
    
	return true;  // This compiles, but may not be correct
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_size;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if(segNum >= m_size) return false;
    
    seg = *m_segments[segNum];
	return true;  
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
