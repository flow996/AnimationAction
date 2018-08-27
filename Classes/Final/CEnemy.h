#ifndef __CENEMY_H__
#define __CENEMY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "SimpleAudioEngine.h"
#include "cocostudio\CocoStudio.h"

USING_NS_CC;
using namespace cocostudio::timeline;	//�ʵe�ɶ��b
using namespace CocosDenshion;			//���ĥ�

class CEnemy: public cocos2d::Node
{
private:
	cocos2d::Node   *_fire1;
	cocos2d::Node   *_fire2;
	cocos2d::Sprite *_shadow;
	cocos2d::CallFunc *_mycallback;
public:
	cocos2d::Node   *_enemyRoot;		//�`�I
	cocos2d::Sprite	*_body;				//�Ϥ�
	cocos2d::Rect	rectenemy;			//�d��
	cocos2d::Point	_enemyPt;			//��m
	cocos2d::Size	size;				//�j�p
	CEnemy(const char *csbname, cocos2d::Layer &parent);
	~CEnemy();
	cocostudio::timeline::ActionTimeline *_action;
	void getRect();						//���oRect
	bool _bDamage;						//�O�_�w���ˡA�O�h���A�P�_	
	bool detect(CEnemy &Ene,Point run);				//�P�_�I��
	void setPosition(const float x, const float y);	//���s�]�w��m
	void go(int dif);					//�]�w��ê�����רò���
};

#endif