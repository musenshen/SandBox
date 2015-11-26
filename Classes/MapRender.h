#ifndef __SANDBOX_MAPRENDER_H__
#define __SANDBOX_MAPRENDER_H__

#include "cocos2d.h"

#define  MAX_TEX_X 3
#define  MAX_TEX_Y 3

namespace maps {
	class Map;
}

class MapRender : public cocos2d::Node
{
public:
	static MapRender* createMapNode(const std::string& fileImage);

public:
	MapRender();
	virtual ~MapRender();

public:
	virtual bool initMapNode(const std::string& fileImage);

protected:
	bool initRenderTexture();
	bool doRenderTexture(maps::Map* mapa, const std::string& fileImage);

private:
	cocos2d::RenderTexture* _renderTexture[MAX_TEX_X][MAX_TEX_Y];
};

#endif // __SANDBOX_MAPRENDER_H__
