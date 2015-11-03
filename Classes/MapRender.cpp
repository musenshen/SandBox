#include "MapRender.h"
#include "Map.h"

USING_NS_CC;

const int WIDTH = 4096;
const int HEIGHT = 4096;


/*
struct Biome{
	enum Type{
		Snow,
		Tundra,
		Mountain,
		Taiga,
		Shrubland,
		TemperateDesert,
		TemperateRainForest,
		TemperateDeciduousForest,
		Grassland,
		TropicalRainForest,
		TropicalSeasonalForest,
		SubtropicalDesert,
		Ocean,
		Lake,
		Beach,

		Size,
		None
	};
};
*/
const Rect BIOME_TEXTURE[] = {
	Rect(16, 336, 32, 32),			// Snow
	Rect(208, 144, 32, 32),			// Tundra
	Rect(144, 144, 32, 32),			// Mountain
	Rect(80, 144, 32, 32),			// Taiga
	Rect(272, 240, 32, 32),			// Shrubland
	Rect(16, 144, 32, 32),			// TemperateDesert
	Rect(80, 240, 32, 32),			// TemperateRainForest
	Rect(272, 48, 32, 32),			// TemperateDeciduousForest
	Rect(208, 48, 32, 32),			// Grassland
	Rect(80, 48, 32, 32),			// TropicalRainForest
	Rect(80, 240, 32, 32),			// TropicalSeasonalForest
	Rect(208, 240, 32, 32),			// SubtropicalDesert
	Rect(336, 144, 32, 32),			// Ocean
	Rect(336, 48, 32, 32),			// Lake
	Rect(16, 240, 32, 32)			// Beach
};


static long  GetCocos2dTime()
{
	struct  timeval tv;
	cocos2d::gettimeofday(&tv, NULL);
	return  tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


MapRender* MapRender::createMapNode(const std::string& fileImage)
{
	MapRender *batchNode = new (std::nothrow) MapRender();
	if (batchNode != nullptr)
	{
		batchNode->initWithFile(fileImage);
		batchNode->initMapNode();
		batchNode->autorelease();
	}

	return batchNode;
}

MapRender::MapRender()
: _reusedTile(nullptr)
{

}

MapRender::~MapRender()
{

}

// on "init" you need to initialize your instance
bool MapRender::initMapNode()
{
	this->setContentSize(Size(WIDTH, HEIGHT));

	long pre = GetCocos2dTime();
	long now = pre;

	CCLOG("Map Generate: ");

	maps::Map mapa(WIDTH, HEIGHT, 10, "");
	mapa.GenerateTest();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	vector<maps::edge *> edges = mapa.GetEdges();
	vector<maps::corner *> corners = mapa.GetCorners();
	vector<maps::center *> centers = mapa.GetCenters();

	size_t len = WIDTH / TILE_SIZE;
	vector<maps::center *>::iterator center_iter = centers.begin();
	for (; center_iter != centers.end(); center_iter++)
	{
		Sprite* tile = reusedTileWithRect(BIOME_TEXTURE[(*center_iter)->biome]);

		size_t index = (*center_iter)->index;
		tile->setPosition(cocos2d::Vec2((*center_iter)->position.x, (*center_iter)->position.y));
		tile->setAnchorPoint(cocos2d::Vec2::ZERO);

		this->insertQuadFromSprite(tile, index);
	}

    return true;
}

cocos2d::Sprite* MapRender::reusedTileWithRect(cocos2d::Rect rect)
{
	if (!_reusedTile)
	{
		_reusedTile = Sprite::createWithTexture(_textureAtlas->getTexture(), rect);
		_reusedTile->setBatchNode(this);
		_reusedTile->retain();
	}
	else
	{
		// FIXME: HACK: Needed because if "batch node" is nil,
		// then the Sprite'squad will be reset
		_reusedTile->setBatchNode(nullptr);

		// Re-init the sprite
		_reusedTile->setTextureRect(rect, false, rect.size);

		// restore the batch node
		_reusedTile->setBatchNode(this);
	}

	return _reusedTile;
}