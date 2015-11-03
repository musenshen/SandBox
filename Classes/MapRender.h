#ifndef __SANDBOX_MAPRENDER_H__
#define __SANDBOX_MAPRENDER_H__

#include "cocos2d.h"

class MapRender : public cocos2d::SpriteBatchNode
{
public:
	static MapRender* createMapNode(const std::string& fileImage);

public:
	MapRender();
	virtual ~MapRender();

public:
	virtual bool initMapNode();

protected:
	cocos2d::Sprite* reusedTileWithRect(cocos2d::Rect rect);

private:
	//! used for optimization
	cocos2d::Sprite *_reusedTile;
};

#endif // __SANDBOX_MAPRENDER_H__
