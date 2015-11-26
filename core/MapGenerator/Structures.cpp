
#include "Structures.h"
#include "Math/LineEquation.h" 
#include <iostream>

namespace maps {

Center::Center()
:_index(0), _row(0), _col(0), _position(0, 0), _border(false), _terrain(Terrain::None), _biome(Biome::None), _elevation(0.0) 
{
}

Center::Center(unsigned int i, unsigned int r, unsigned int c, Vec2 p)
: _index(i), _row(r), _col(c), _position(p), _border(false), _terrain(Terrain::None), _biome(Biome::None), _elevation(0.0) 
{
}

Center::~Center()
{

}

void Center::setIndex(unsigned int i)
{
	_index = i;
}

unsigned int Center::getIndex()
{
	return _index;
}

void Center::setRow(unsigned int r)
{
	_row = r;
}

unsigned int Center::getRow()
{
	return _row;
}

void Center::setCol(unsigned int c)
{
	_col = c;
}

unsigned int Center::getCol()
{
	return _col;
}

void Center::setPosition(const Vec2 &position)
{
	_position = position;
}

const Vec2& Center::getPosition() const
{
	return _position;
}

void Center::setBorder(bool border)
{
	_border = border;
}

bool Center::isBorder()
{
	return _border;
}

void Center::setTerrain(Terrain terrain)
{
	_terrain = terrain;
}

Terrain Center::getTerrain()
{
	return _terrain;
}

void Center::setTerrainIndex(unsigned int i)
{
	_terrainIndex = i;
}

unsigned int Center::getTerrainIndex()
{
	return _terrainIndex;
}

void Center::setBiome(Biome biome)
{
	_biome = biome;
}

maps::Biome Center::getBiome()
{
	return _biome;
}

void Center::setBiomeIndex(unsigned int i)
{
	_biomeIndex = i;
}

unsigned int Center::getBiomeIndex()
{
	return _biomeIndex;
}

void Center::setElevation(double elevation)
{
	_elevation = elevation;
}

double Center::getElevation()
{
	return _elevation;
}

vector<Center*> Center::getCenters()
{
	return _centers;
}

}