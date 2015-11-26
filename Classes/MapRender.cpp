#include "MapRender.h"
#include "Map.h"

USING_NS_CC;

const int WIDTH = 12288;
const int HEIGHT = 12288;


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
	MapRender *node = new (std::nothrow) MapRender();
	if (node != nullptr)
	{
		node->init();
		node->initMapNode(fileImage);
		node->autorelease();
		node->retain();
	}

	return node;
}

MapRender::MapRender()
{

}

MapRender::~MapRender()
{
	for (size_t i = 0; i < MAX_TEX_X; i++)
	{
		for (size_t j = 0; j < MAX_TEX_Y; j++)
		{
			RenderTexture* tex = _renderTexture[i][j];
			if (tex) tex->release();
		}
	}
}

// on "init" you need to initialize your instance
bool MapRender::initMapNode(const std::string& fileImage)
{
	setContentSize(Size(WIDTH, HEIGHT));
	initRenderTexture();

	long pre = GetCocos2dTime();
	long now = pre;

	CCLOG("Map Generate: ");

	maps::Map* mapa = new maps::Map(WIDTH, HEIGHT, 10, "");
	mapa->GenerateTest();

	now = GetCocos2dTime();
	CCLOG("%ld ms.", now - pre);
	pre = now;

	doRenderTexture(mapa, fileImage);

    return true;
}

bool MapRender::initRenderTexture()
{
	// create a render texture, this is what we are going to draw into
	int perWidth = WIDTH / MAX_TEX_X;
	int perHeight = HEIGHT / MAX_TEX_Y;
	for (size_t i = 0; i < MAX_TEX_X; i++)
	{
		for (size_t j = 0; j < MAX_TEX_Y; j++)
		{
			RenderTexture* renderTexture = RenderTexture::create(perWidth, perHeight, Texture2D::PixelFormat::RGBA8888);
			renderTexture->retain();
			renderTexture->setPosition(cocos2d::Vec2(perWidth / 2 + i * perWidth, perHeight / 2 + j * perHeight));

			this->addChild(renderTexture);
			_renderTexture[i][j] = renderTexture;
		}
	}
	return true;
}

bool MapRender::doRenderTexture(maps::Map* mapa, const std::string& fileImage)
{
	vector<maps::center *> centers = mapa->GetCenters();
	int perWidth = WIDTH / MAX_TEX_X;
	int perHeight = HEIGHT / MAX_TEX_Y;

	vector<maps::center *> unitCenters[MAX_TEX_X][MAX_TEX_Y];
	vector<maps::center *>::iterator center_iter = centers.begin();
	for (; center_iter != centers.end(); center_iter++)
	{
		int x = (*center_iter)->_row * TILE_SIZE / perWidth;
		int y = (*center_iter)->_col * TILE_SIZE / perHeight;
		unitCenters[x][y].push_back(*center_iter);
	}

	for (size_t i = 0; i < MAX_TEX_X; i++)
	{
		for (size_t j = 0; j < MAX_TEX_Y; j++)
		{
			RenderTexture* render = _renderTexture[i][j];
			// begin drawing to the render texture
			render->begin();

			vector<maps::center *> curCenters = unitCenters[i][j];
			center_iter = curCenters.begin();
			for (; center_iter != curCenters.end(); center_iter++)
			{
				Sprite* tile = Sprite::create(fileImage, TERRAIN_TEXTURE[(*center_iter)->_terrain]);

				size_t index = (*center_iter)->_index;
				tile->setPosition(cocos2d::Vec2((*center_iter)->_position.x - i * perWidth, (*center_iter)->_position.y - j * perHeight));
				tile->setAnchorPoint(cocos2d::Vec2::ZERO);

				tile->visit();
			}


			// finish drawing and return context back to the screen
			render->end();
		}
	}

	return true;
}
