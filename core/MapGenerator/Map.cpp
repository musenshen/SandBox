#include "Map.h"
#include "Math/Vec2.h"
#include <ctime>
#include <queue>
#include "cocos2d.h"

namespace maps {

static long  GetCocos2dTime()
{
	struct  timeval tv;
	cocos2d::gettimeofday(&tv, NULL);
	return  tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

const vector<vector<Biome::Type> > Map::elevation_moisture_matrix = Map::MakeBiomeMatrix();

vector<vector<Biome::Type> > Map::MakeBiomeMatrix(){
	vector<vector<Biome::Type> > matrix;
	vector<Biome::Type> biome_vector;

	biome_vector.push_back(Biome::SubtropicalDesert);
	biome_vector.push_back(Biome::TemperateDesert);
	biome_vector.push_back(Biome::TemperateDesert);
	biome_vector.push_back(Biome::Mountain);
	matrix.push_back(biome_vector);

	biome_vector.clear();
	biome_vector.push_back(Biome::Grassland);
	biome_vector.push_back(Biome::Grassland);
	biome_vector.push_back(Biome::TemperateDesert);
	biome_vector.push_back(Biome::Mountain);
	matrix.push_back(biome_vector);

	biome_vector.clear();
	biome_vector.push_back(Biome::TropicalSeasonalForest);
	biome_vector.push_back(Biome::Grassland);
	biome_vector.push_back(Biome::Shrubland);
	biome_vector.push_back(Biome::Tundra);
	matrix.push_back(biome_vector);

	biome_vector.clear();
	biome_vector.push_back(Biome::TropicalSeasonalForest);
	biome_vector.push_back(Biome::TemperateDeciduousForest);
	biome_vector.push_back(Biome::Shrubland);
	biome_vector.push_back(Biome::Snow);
	matrix.push_back(biome_vector);

	biome_vector.clear();
	biome_vector.push_back(Biome::TropicalRainForest);
	biome_vector.push_back(Biome::TemperateDeciduousForest);
	biome_vector.push_back(Biome::Taiga);
	biome_vector.push_back(Biome::Snow);
	matrix.push_back(biome_vector);

	biome_vector.clear();
	biome_vector.push_back(Biome::TropicalRainForest);
	biome_vector.push_back(Biome::TemperateRainForest);
	biome_vector.push_back(Biome::Taiga);
	biome_vector.push_back(Biome::Snow);
	matrix.push_back(biome_vector);

	return matrix;
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
	CCLOG("Seed: %s (%u)", m_seed, HashString(m_seed));
	
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
			center* c = new center(index++, Vec2(i * TILE_SIZE, j*TILE_SIZE));
			c->biome = (Biome::Type)(rand() % 15);
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

	// Establezco los bordes del mapa
	for each(corner * c in corners) {
		if(!c->IsInsideBoundingBox(map_width, map_height)){
			c->border = true;
			c->ocean = true;
			c->water = true;
		}
	}

	// Determino lo que es agua y lo que es tierra
	for each (corner * c in corners) {
		c->water = !IsIsland(c->position);
	}
}

void Map::AssignOceanCoastLand(){
	queue<center *> centers_queue;
	// Quien es agua o border
	for each (center * c in centers) {
		int adjacent_water = 0;
		for each (corner * q in c->corners) {
			if(q->border){
				c->border = true;
				c->ocean = true;
				q->water = true;
				centers_queue.push(c);
			}
			if(q->water){
				adjacent_water++;
			}
		}
		c->water = (c->ocean || adjacent_water >= c->corners.size() * 0.5);
	}

	// Quien es oceano y quien no
	while(!centers_queue.empty()){
		center * c = centers_queue.front();
		centers_queue.pop();
		for each (center * r in c->centers)	{
			if(r->water && !r->ocean){
				r->ocean = true;
				centers_queue.push(r);
			}
		}
	}

	// Costas de center
	for each (center * p in centers) {
		int num_ocean = 0;
		int num_land = 0;
		for each (center * q in p->centers) {
			num_ocean += q->ocean;
			num_land += !q->water;
		}
		p->coast = num_land > 0 && num_ocean > 0;
	}

	// Costas de corner
	for each (corner * c in corners) {
		int adj_ocean = 0;
		int adj_land = 0;
		for each (center * p in c->centers) {
			adj_ocean += (int) p->ocean;
			adj_land += (int) !p->water;
		}
		c->ocean = adj_ocean == c->centers.size();
		c->coast = adj_land > 0 && adj_ocean > 0;
		c->water = c->border || (adj_land != c->centers.size() && !c->coast);
	}
}

void Map::AssignCornerElevation(){
	queue<corner *> corner_queue;
	for each (corner * q in corners) {
		if(q->border){
			q->elevation = 0.0;
			corner_queue.push(q);
		}else{
			q->elevation = 99999;
		}
	}

	while(!corner_queue.empty()){
		corner * q = corner_queue.front();
		corner_queue.pop();

		for each (corner * s in q->corners) {
			double new_elevation = q->elevation + 0.01;
			if(!q->water && !s->water){
				new_elevation += 1;
			}
			if(new_elevation < s->elevation){
				s->elevation = new_elevation;
				corner_queue.push(s);
			}
		}
	}

	for each (corner * q in corners) {
		if(q->water){
			q->elevation = 0.0;
		}
	}
}

void Map::RedistributeElevations(){
	vector<corner *> locations = GetLandCorners();
	double SCALE_FACTOR = 1.05;

	sort(locations.begin(), locations.end(), &corner::SortByElevation);

	for(int i = 0; i < locations.size(); i++){
		double y = (double) i / (locations.size() - 1);
		double x = sqrt(SCALE_FACTOR) - sqrt(SCALE_FACTOR * (1-y));
		x = min(x, 1.0);
		locations[i]->elevation = x;
	}
}

void Map::AssignPolygonElevations(){
	for each (center * p in centers) {
		double elevation_sum = 0.0;
		for each (corner * q in p->corners) {
			elevation_sum += q->elevation;
		}
		p->elevation = elevation_sum / p->corners.size();
	}
}

void Map::CalculateDownslopes(){
	for each (corner * c in corners) {
		corner * d = c;
		for each (corner * q in c->corners) {
			if(q->elevation < d->elevation){
				d = q;
			}
		}
		c->downslope = d;
	}
}

void Map::GenerateRivers(){
	//int num_rios = (map_height + map_width) / 4;
	int num_rios = centers.size() / 3;
	for(int i = 0; i < num_rios; i++){
		corner *q = corners[rand()%corners.size()];
		if( q->ocean || q->elevation < 0.3 || q->elevation > 0.9 ) continue;

		while(!q->coast){
			if(q == q->downslope){
				break;
			}
			edge * e = q->GetEdgeWith(q->downslope);
			e->river_volume += 1;
			q->river_volume += 1;
			q->downslope->river_volume += 1;
			q = q->downslope;
		}
	}
}

void Map::AssignCornerMoisture(){
	queue<corner *> corner_queue;
	// Agua dulce
	for each (corner * c in corners) {
		if((c->water || c->river_volume > 0) && !c->ocean){
			c->moisture = c->river_volume > 0 ? min(3.0, (0.2 * c->river_volume)) : 1.0;
			corner_queue.push(c);
		}else{
			c->moisture = 0.0;
		}
	}

	while(!corner_queue.empty()){
		corner * c = corner_queue.front();
		corner_queue.pop();

		for each (corner * r in c->corners)	{
			double new_moisture = c->moisture * 0.9;
			if( new_moisture > r->moisture ){
				r->moisture = new_moisture;
				corner_queue.push(r);
			}
		}
	}

	// Agua salada
	for each (corner * r in corners) {
		if(r->ocean){
			r->moisture = 1.0;
			corner_queue.push(r);
		}
	}
	while(!corner_queue.empty()){
		corner * c = corner_queue.front();
		corner_queue.pop();

		for each (corner * r in c->corners)	{
			double new_moisture = c->moisture * 0.3;
			if( new_moisture > r->moisture ){
				r->moisture = new_moisture;
				corner_queue.push(r);
			}
		}
	}

}

void Map::RedistributeMoisture(){
	vector<corner *> locations = GetLandCorners();

	sort(locations.begin(), locations.end(), &corner::SortByMoisture);

	for(int i = 0; i < locations.size(); i++){
		locations[i]->moisture = (double) i / (locations.size() - 1);
	}
}

void Map::AssignPolygonMoisture(){
	for each (center * p in centers) {
		double new_moisture = 0.0;
		for each (corner * q in p->corners){
			if(q->moisture > 1.0) q->moisture = 1.0;
			new_moisture += q->moisture;
		}
		p->moisture = new_moisture / p->corners.size();
	}
}

void Map::AssignBiomes(){

	for each (center * c in centers) {
		if(c->ocean){
			c->biome = Biome::Ocean;
		}else if(c->water){
			c->biome = Biome::Lake;
		}else if(c->coast && c->moisture < 0.6){
			c->biome = Biome::Beach;
		}else{
			int elevation_index = 0;
			if(c->elevation > 0.85){
				elevation_index = 3;
			}else if(c->elevation > 0.6){
				elevation_index = 2;
			}else if(elevation_index > 0.3){
				elevation_index = 1;
			}else{
				elevation_index = 0;
			}

			int moisture_index = min((int) floor(c->moisture * 6), 5);
			c->biome = elevation_moisture_matrix[moisture_index][elevation_index];
		}
	}
}

void Map::FinishInfo(){
	center::PVIter center_iter, centers_end = centers.end();
	for(center_iter = centers.begin(); center_iter != centers_end; center_iter++){
		(*center_iter)->SortCorners();
	}

	for each (center * c  in centers) {
		for each (edge * e in c->edges) {
			center *aux_center = e->GetOpositeCenter(c);
			if(aux_center != NULL)
				c->centers.push_back(aux_center);
		}
	}
	for each (corner * c  in corners) {
		for each (edge * e in c->edges) {
			corner * aux_corner = e->GetOpositeCorner(c);
			if(aux_corner != NULL)
				c->corners.push_back(aux_corner);
		}
	}
}

vector<corner *> Map::GetLandCorners(){
	vector<corner *> land_corners;
	for each (corner * c in corners)
		if(!c->water)
			land_corners.push_back(c);
	return land_corners;
}

vector<corner *> Map::GetLakeCorners(){
	vector<corner *> lake_corners;
	for each (corner * c in corners)
		if(c->water && !c->ocean)
			lake_corners.push_back(c);
	return lake_corners;
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

void Map::AddCenter(center * c){
	map<double, map<double, center *> >::const_iterator it = pos_cen_map.find(c->position.x);
	if(it != pos_cen_map.end()){
		pos_cen_map[(int) c->position.x][c->position.y] = c;
	}else{
		pos_cen_map[(int) c->position.x] = map<double, center*>();
		pos_cen_map[(int) c->position.x][c->position.y] = c;
	}
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
	vector<del::vertex> new_points;
	for each (center * p in centers) {
		if(!p->IsInsideBoundingBox(map_width, map_height)){
			new_points.push_back(del::vertex((int)p->position.x, (int) p->position.y));
			continue;
		}
		Vec2 center_centroid;		
		for each (corner * q in p->corners)	{
			if(q->IsInsideBoundingBox(map_width, map_height)){
				center_centroid += q->position;
			}else{
				Vec2 corner_pos = q->position;
				if(corner_pos.x < 0){
					corner_pos.x = 0;
				}else if(corner_pos.x >= map_width){
					corner_pos.x = map_width;
				}
				if(corner_pos.y < 0){
					corner_pos.y = 0;
				}else if(corner_pos.y >= map_height){
					corner_pos.y = map_height;
				}
				center_centroid += corner_pos;
			}
		}
		center_centroid /= p->corners.size();
		new_points.push_back(del::vertex((int)center_centroid.x, (int)center_centroid.y));
	}
	Triangulate(new_points);
}

vector<center *> Map::GetCenters(){
	return centers;
}

vector<corner *> Map::GetCorners(){
	return corners;
}

vector<edge *> Map::GetEdges(){
	return edges;
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