#include "Map.h"
#include "Math/Vec2.h"
#include <ctime>
#include <queue>
#include "cocos2d.h"

namespace maps {

static long  GetCocos2dTime()
{
	return cocos2d::utils::getTimeInMilliseconds();
}

Map::Map(void) {}

Map::~Map(void) {}

Map::Map(int width, int height, double point_spread, string seed){
	map_width = width;
	map_height = height;
	m_point_spread = point_spread;

	double l_aprox_point_count = (2 * map_width * map_height) / (3.1416 * point_spread * point_spread);
	int l_max_tree_depth = floor((log(l_aprox_point_count) / log(4)) + 0.5);

	m_seed = seed != "" ? seed : CreateSeed(20);	
	srand(HashString(m_seed));

	z_coord = rand();
	CCLOG("Seed: %s (%u)", m_seed.c_str(), HashString(m_seed));
	
}

void Map::GenerateTest()
{
	size_t index = 0;
	size_t lenX = map_width / TILE_SIZE;
	size_t lenY = map_height / TILE_SIZE;
	for (size_t j = 0; j < lenY; j++)
	{
		for (size_t i = 0; i < lenX; i++)
		{
			center* c = new center(index++, i, j, Vec2(i * TILE_SIZE, j*TILE_SIZE));
			c->_terrain = (Terrain::Type)(rand() % Terrain::Size);
			centers.push_back(c);
		}
	}
}

void Map::Generate() {
	long pre = GetCocos2dTime();
	long now = pre;
	CCLOG("GeneratePolygons: ");

	GeneratePolygons();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now-pre);
	pre = now;

	CCLOG("Land distribution: ");

	// GenerateLand
	GenerateLand();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	// ELEVATION
	CCLOG("Coast assignment: ");

	// AssignOceanCoastLand
	AssignOceanCoastLand();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	CCLOG("Corner altitude: ");

	// AssignCornerElevation
	AssignCornerElevation();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	CCLOG("Altitude redistribution: ");

	// RedistributeElevations
	RedistributeElevations();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	CCLOG("Center altitude: ");

	// AssignPolygonElevations
	AssignPolygonElevations();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	// MOISTURE
	CCLOG("Downslopes: ");

	// CalculateDownslopes
	CalculateDownslopes();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	CCLOG("River generation: ");

	// GenerateRivers
	GenerateRivers();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	CCLOG("Corner moisture: ");

	// AssignCornerMoisture
	AssignCornerMoisture();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	CCLOG("Moisture redistribution: ");

	// RedistributeMoisture
	RedistributeMoisture();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	CCLOG("Center moisture: ");

	// AssignPolygonMoisture
	AssignPolygonMoisture();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	// BIOMES
	CCLOG("Biome assignment: ");

	// AssignBiomes
	AssignBiomes();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;
}

void Map::GeneratePolygons() {
	long pre = GetCocos2dTime();
	long now = pre;
	GeneratePoints();

	now = GetCocos2dTime();
	CCLOG("Point placement: %ld ms.", now - pre);
	pre = now;

	Triangulate(points);

	now = GetCocos2dTime();
	CCLOG("Triangulation: %ld ms.", now - pre);
	pre = now;

	FinishInfo();

	now = GetCocos2dTime();
	CCLOG("Finishing touches: %ld ms.", now - pre);
	pre = now;
}

void Map::GenerateLand() {


}

void Map::AssignOceanCoastLand(){
	
}

void Map::AssignCornerElevation(){
	
}

void Map::RedistributeElevations(){
	}

void Map::AssignPolygonElevations(){
    
}

void Map::CalculateDownslopes(){

}

void Map::GenerateRivers(){
	
}

void Map::AssignCornerMoisture(){
	

}

void Map::RedistributeMoisture(){
	
}

void Map::AssignPolygonMoisture(){
    
}

void Map::AssignBiomes(){

}

void Map::FinishInfo(){
    
}


bool Map::IsIsland(Vec2 position){

	double water_threshold = 0.075;

	if(position.x < map_width * water_threshold || position.y < map_height * water_threshold
		|| position.x > map_width * (1 - water_threshold) || position.y > map_height * (1 - water_threshold))
		return false;

	return true;
	/*
	Vec2 center_pos = Vec2(map_width / 2.0, map_height / 2.0);

	position -= center_pos;
	double x_coord = (position.x / map_width) * 4;
	double y_coord = (position.y / map_height) * 4;
	double noise_val = noiseMap->GetValue(x_coord, y_coord, z_coord);

	position /= min(map_width, map_height);
	double radius = position.Length();

	double factor = radius - 0.5;

	return noise_val >= 0.3*radius + factor;
	*/
}

void Map::Triangulate(vector<del::vertex> puntos){
	

}


center * Map::GetCenter(Vec2 position){
	map<double, map<double, center *> >::const_iterator it = pos_cen_map.find(position.x);
	if(it != pos_cen_map.end()){
		map<double, center *>::const_iterator it2 = it->second.find(position.y);
		if(it2 != it->second.end()){
			return it2->second;
		}
	}

	return NULL;
}

void Map::GeneratePoints(){

}

void Map::LloydRelaxation(){
	
}

vector<center *> Map::GetCenters(){
	return centers;
}

unsigned int Map::HashString(string seed){
	unsigned int hash = 0;
	for(int i = 0; i < seed.length(); i++) {
		hash += ((int) seed[i]) * pow(2, i);
	}
	return hash % UINT_MAX;
}

string Map::CreateSeed(int length){
	srand(time(NULL));
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	string seed;
	for (int i = 0; i < length; ++i) {
		seed.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
	}
	return seed;
}

}