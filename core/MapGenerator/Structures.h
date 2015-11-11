#pragma once

#include "Math/Vec2.h"
#include <vector>
#include <list>

using namespace std;

namespace maps {

struct Terrain{
	enum Type{
		Snow,
		Plain,
		Grassland,
		Swamp,
		Mountain,
		Desert,
		Ocean,
		Lake,
		Beach,	

		Size,
		None
	};
};
    
struct Biome{
    enum Type{
        Tree,
        Bush,
        Bridge,
        House,
            
        Size,
        None
    };
};


// Center of Voronoi cell; Corner of Delaunay triangle
struct center{
	center() : _index(0), _row(0), _col(0), _position(0,0), _border(false), _terrain(Terrain::None), _biome(Biome::None), _elevation(0.0) {}

	center(unsigned int i, unsigned int r, unsigned int c, Vec2 p) : _index(i), _row(r), _col(c), _position(p), _border(false), _terrain(Terrain::None), _biome(Biome::None), _elevation(0.0) {}

	unsigned int		_index;
	unsigned int		_row;
	unsigned int		_col;
	Vec2				_position;
    
	bool _border;
	Terrain::Type _terrain;
    unsigned int  _terrainIndex;
    
    Biome::Type _biome;
    unsigned int  _biomeIndex;
    
	double _elevation;

	vector<center *>	_centers;

	bool IsInsideBoundingBox(int width, int height);
	bool GoesBefore(Vec2 p_a, Vec2 p_b);
    
};

}