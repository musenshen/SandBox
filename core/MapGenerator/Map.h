#pragma once

#include "dDelaunay.h"
#include "Structures.h"
#include <vector>
#include <map>

#define TILE_SIZE 32

// Forward Declarations
class Vec2;

namespace maps {

class Map {
public:
	Map(void);
	~Map(void);

	Map(int width, int height, double point_spread, string seed);

	void Generate();
	void GenerateTest();

	void GeneratePolygons();
	void GenerateLand();

	bool LoadFile(string file_name);
	bool WriteFile(string file_name);

	vector<edge *> GetEdges();
	vector<corner *> GetCorners();
	vector<center *> GetCenters();

private:
	int map_width;
	int map_height;
	double m_point_spread;
	double z_coord;
	string m_seed;

	vector<del::vertex> points;

	map<double, map<double, center *> > pos_cen_map;
	vector<edge *> edges;
	vector<corner *> corners;
	vector<center *> centers;

	static const vector<vector<Biome::Type> > elevation_moisture_matrix;
	static vector<vector<Biome::Type> > MakeBiomeMatrix();

	bool IsIsland(Vec2 position);
	void AssignOceanCoastLand();
	void AssignCornerElevation();
	void RedistributeElevations();
	void AssignPolygonElevations();
	void CalculateDownslopes();
	void GenerateRivers();
	void AssignCornerMoisture();
	void RedistributeMoisture();
	void AssignPolygonMoisture();
	void AssignBiomes();

	void GeneratePoints();
	void Triangulate(vector<del::vertex> puntos);
	void FinishInfo();
	void AddCenter(center * c);
	center * GetCenter(Vec2 position);
	void OrderPoints(vector<corner *> &corners);

	vector<corner *> GetLandCorners();
	vector<corner *> GetLakeCorners();
	void LloydRelaxation();
	static unsigned int HashString(string seed);
	string CreateSeed(int length);
};

}