//#ifndef __C321GO_H__
//#define __C321GO_H__

#include "cocos2d.h"

using namespace cocos2d;

class c321go :public cocos2d::Layer
{
private:
	int idt;
	CCSprite *_SpriteCD[4];
	float _tot;

public:
	c321go();
	~c321go();
	void init(cocos2d::Point &pt, cocos2d::Layer &parent);
	bool doStep(float dt);
	void reset(cocos2d::Point &pt, cocos2d::Layer &parent);
};	
//#endif