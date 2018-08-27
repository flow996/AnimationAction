#include "c321go.h"

c321go::c321go() {
	idt = 3;
	_tot = 0;
}
c321go::~c321go() {

}

void c321go::init(cocos2d::Point &pt,cocos2d::Layer &parent) {
	_SpriteCD[3] = Sprite::createWithSpriteFrameName("3.png");
	_SpriteCD[2] = Sprite::createWithSpriteFrameName("2.png");
	_SpriteCD[1] = Sprite::createWithSpriteFrameName("1.png");
	_SpriteCD[0] = Sprite::createWithSpriteFrameName("0.png");
	for (size_t i = 0; i < 4; i++)
	{
		_SpriteCD[i]->setPosition(pt);
		parent.addChild(_SpriteCD[i]);
		this->addChild(_SpriteCD[i], 1);
		_SpriteCD[i]->setVisible(false);
	}
}

bool c321go::doStep(float dt) {
	_SpriteCD[idt]->setVisible(true);
	if (_tot >= 1.0f)
	{
		_SpriteCD[idt]->setVisible(false);
		_SpriteCD[idt]->setScale(1.0f);
		_tot -= 1.0f;
		idt--;
		if (idt == -1){
			return true;
		}
		else{
			_SpriteCD[idt]->setVisible(true);
		}	
	}
	else{
		float t = sin(_tot * 180 * 3.1415926 / 180.0f);
		_SpriteCD[idt]->setScale(1 + t);
	}
	_tot += dt;
			return (false);
}
void c321go::reset(cocos2d::Point &pt, cocos2d::Layer &parent) {
	idt = 3;
	_tot = 0;
	for (size_t i = 0; i < 4; i++)
	{
		_SpriteCD[i]->setPosition(pt);
		parent.addChild(_SpriteCD[i]);
		this->addChild(_SpriteCD[i], 1);
		_SpriteCD[i]->setVisible(false);
	}
}