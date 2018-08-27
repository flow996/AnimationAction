#ifndef __CBEAN_H__
#define __CBEAN_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace CocosDenshion;

class CBean
{
private:
	cocos2d::Sprite *_shadow;
	// For Double Jump
public:
	cocos2d::Node   *_enemyRoot;
	cocos2d::Sprite *_bean1;
	cocos2d::Sprite *_bean2;
	cocos2d::Sprite *_bean3;
	cocos2d::Sprite *_bean4;

	cocos2d::Rect   rectenemy;//½d³ò
	cocos2d::Point	_enemyPt;//¦ì¸m
	cocos2d::Size  size;

	cocostudio::timeline::ActionTimeline *_action;
	CBean(const char *csbname, cocos2d::Layer &parent);
	~CBean();
	void getrect(float dt);
	//void getPosition();
	void setPosition(const float x, const float y);
	void go(int dif);
	void stop();
};

#endif