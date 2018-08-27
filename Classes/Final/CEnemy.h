#ifndef __CENEMY_H__
#define __CENEMY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "SimpleAudioEngine.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;
using namespace cocostudio::timeline;	//動畫時間軸
using namespace CocosDenshion;			//音效用

class CEnemy: public cocos2d::Node
{
private:
	cocos2d::Node   *_fire1;
	cocos2d::Node   *_fire2;
	cocos2d::Sprite *_shadow;
	cocos2d::CallFunc *_mycallback;
public:
	cocos2d::Node   *_enemyRoot;		//節點
	cocos2d::Sprite	*_body;				//圖片
	cocos2d::Rect	rectenemy;			//範圍
	cocos2d::Point	_enemyPt;			//位置
	cocos2d::Size	size;				//大小
	CEnemy(const char *csbname, cocos2d::Layer &parent);
	~CEnemy();
	cocostudio::timeline::ActionTimeline *_action;
	void getRect();						//取得Rect
	bool _bDamage;						//是否已受傷，是則不再判斷	
	bool detect(CEnemy &Ene,Point run);				//判斷碰撞
	void setPosition(const float x, const float y);	//重新設定位置
	void go(int dif);					//設定障礙物難度並移動
};

#endif