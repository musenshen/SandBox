#include "MapRender.h"
#include "Map.h"

USING_NS_CC;

const int WIDTH = 4096;
const int HEIGHT = 4096;


/*
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
*/
const Rect TERRAIN_TEXTURE[] = {
	Rect(16, 336, 32, 32),			// Snow
	Rect(208, 144, 32, 32),			// Plain
	Rect(208, 48, 32, 32),			// Grassland
	Rect(272, 144, 32, 32),			// Swamp
	Rect(144, 144, 32, 32),			// Mountain
	Rect(208, 240, 32, 32),			// Desert
	Rect(336, 144, 32, 32),			// Ocean
	Rect(336, 48, 32, 32),			// Lake
	Rect(16, 240, 32, 32)			// Beach
};


static long  GetCocos2dTime()
{
    return cocos2d::utils::getTimeInMilliseconds();
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

	maps::Map* mapa = new maps::Map(WIDTH, HEIGHT, 10, "");
	mapa->GenerateTest();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	vector<maps::center *> centers = mapa->GetCenters();

	size_t len = WIDTH / TILE_SIZE;
	vector<maps::center *>::iterator center_iter = centers.begin();
	for (; center_iter != centers.end(); center_iter++)
	{
		Sprite* tile = reusedTileWithRect(TERRAIN_TEXTURE[(*center_iter)->_terrain]);

		size_t index = (*center_iter)->_index;
		tile->setPosition(cocos2d::Vec2((*center_iter)->_position.x, (*center_iter)->_position.y));
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