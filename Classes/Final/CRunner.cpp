#include "CRunner.h"

USING_NS_CC;

//
// 建立角色之前，必須先確定已經讀入儲存角色所有動態圖片的 plist 檔
//

CRunner::CRunner(const char *csbname, cocos2d::Layer &parent)
{
	// 讀取角色
	_runnerRoot = CSLoader::createNode(csbname);
//	_runnerRoot = (cocos2d::Node*)getNode->getChildByName("RunnerRoot");
	_runnerRoot->setPosition(0,0); // 預設放在 (0,0) 
	_body = (cocos2d::Sprite *)_runnerRoot->getChildByName("body_cuber01");
	_shadow = (cocos2d::Sprite *)_runnerRoot->getChildByName("shadow_cuber01");
	_normalFace = (cocos2d::Node *)_runnerRoot->getChildByName("NormalFace");
	_normalFace = (cocos2d::Sprite *)_runnerRoot->getChildByName("NormalFace");
	_happyFace = (cocos2d::Node *)_runnerRoot->getChildByName("HappyFace");
	_depressedFace = (cocos2d::Node *)_runnerRoot->getChildByName("DepressedFace");
	init();
	parent.addChild(_runnerRoot);
}

void CRunner::getRect() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	size =_body->getContentSize();//抓大小
	_runnerPt = _runnerRoot->getPosition();//抓節點
	rectrunner = Rect(_runnerPt.x, _runnerPt.y, size.width, size.height);
}

void CRunner::init() {
	_body->setColor(Color3B(255, 255, 255));
	_happyFace->setVisible(false);
	_normalFace->setVisible(true);
	_depressedFace->setVisible(false);
}

CRunner::~CRunner()
{
}

void CRunner::go()
{
	_runnerRoot->setVisible(true);
	_body->runAction(_action);
}
void CRunner::stop() {
	_body->stopAction(_action);
}
void CRunner::setAnimation(const char *plistFile)
{
	// 此處假設儲存角色所有動態圖片的 plist 檔已經讀入
	auto cache = AnimationCache::getInstance();
	cache->addAnimationsWithFile(plistFile);
	auto animation = cache->getAnimation("running");
	_action = RepeatForever::create(Animate::create(animation));
	_action->retain();
}

void CRunner::setPosition(const Point &pos)
{
	_runnerRoot->setPosition(pos);
}

void CRunner::setPosition(const float x, const float y)
{
	_runnerRoot->setPosition(x,y);
}

void CRunner::setColor(const Color3B& color)
{
	_body->setColor(color);
}
