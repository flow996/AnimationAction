#include "CBean.h"

USING_NS_CC;

//
// 建立角色之前，必須先確定已經讀入儲存角色所有動態圖片的 plist 檔
//

CBean::CBean(const char *csbname, cocos2d::Layer &parent)
{
	// 讀取角色
	_enemyRoot = CSLoader::createNode(csbname);
//	_enemyRoot = (cocos2d::Node*)getNode->getChildByName("RunnerRoot");
	_enemyRoot->setPosition(0,0); // 預設放在 (0,0) 
	_bean1 = (cocos2d::Sprite *)_enemyRoot->getChildByName("bean1");
	_bean1->setColor(Color3B(255, 255, 255)); 
	_bean2 = (cocos2d::Sprite *)_enemyRoot->getChildByName("bean2");
	_bean2->setColor(Color3B(255, 255, 255));	
	_bean3 = (cocos2d::Sprite *)_enemyRoot->getChildByName("bean3");
	_bean3->setColor(Color3B(255, 255, 255));	
	_bean4 = (cocos2d::Sprite *)_enemyRoot->getChildByName("bean4");
	_bean4->setColor(Color3B(255, 255, 255));
	_shadow = (cocos2d::Sprite *)_enemyRoot->getChildByName("trishadow_1");
	_action = (ActionTimeline *)CSLoader::createTimeline(csbname);
	_action->retain();

	parent.addChild(_enemyRoot);
}
CBean::~CBean()
{
	_enemyRoot->removeAllChildren();
	_action->release();
}
void CBean::go(int dif)
{
	float r = random(1,4);
	MoveBy * moveby = MoveBy::create(r, Point(1380, 0));
	_enemyRoot->runAction(Sequence::create(moveby, NULL));
	_enemyRoot->runAction(_action);
	_action->setTimeSpeed(1);
	if (dif == 1)//不同的障礙物
	{
		_bean1->setVisible(true);
		_bean2->setVisible(false);
		_bean3->setVisible(false);
		_bean4->setVisible(false);
		_action->gotoFrameAndPlay(0, 20, true);
	}
	else if (dif == 2)
	{
		_bean1->setVisible(false);
		_bean2->setVisible(true);
		_bean3->setVisible(false);
		_bean4->setVisible(false);
		_action->gotoFrameAndPlay(0, 15, true);

	}
	else if(dif == 3)
	{	_bean1->setVisible(false);
		_bean2->setVisible(false);
		_bean3->setVisible(true);
		_bean4->setVisible(false);
		_action->gotoFrameAndPlay(0, 12, true);
		
	}
	else
	{	_bean1->setVisible(false);
		_bean2->setVisible(false);
		_bean3->setVisible(false);
		_bean4->setVisible(true);
		_action->gotoFrameAndPlay(0, 20, true);
	}
}


void CBean::stop() {
	_enemyRoot->stopAction(_action);
}

void CBean::setPosition(const float x, const float y)
{
	_enemyRoot->setPosition(x,y);
}


