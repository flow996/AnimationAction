#ifndef __CRUNNER_H__
#define __CRUNNER_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

#define RUNNERCOLOR1 cocos2d::Color3B(209,73,63)
#define RUNNERCOLOR2 cocos2d::Color3B(200,209,63)
#define RUNNERCOLOR3 cocos2d::Color3B(75,155,77)
#define RUNNERCOLOR4 cocos2d::Color3B(82,131,151);

#define DOUBLEJUMP 1
class CRunner	{
	private:
		// For Double Jump	
		cocos2d::Action *_action;
		cocos2d::JumpBy   *_myJump;
		cocos2d::CallFunc *_mycallback;
	public:
		cocos2d::Node   *_runnerRoot;
		cocos2d::Sprite *_body;
		cocos2d::Sprite *_shadow;

		cocos2d::Node   *_normalFace;
		cocos2d::Node   *_happyFace;
		cocos2d::Node   *_depressedFace;

		cocos2d::Size  size;
		cocos2d::Rect rectrunner;	//½d³ò
		cocos2d::Point _runnerPt;	//¦ì¸m§ì¸`ÂI
		void getRect();

		CRunner(const char *csbname, cocos2d::Layer &parent);
		void init();
		~CRunner();

		void setPosition(const cocos2d::Point &pos);
		void setPosition(const float x, const float y);
		void setColor(const cocos2d::Color3B& color);
		void setAnimation(const char *plistFile);
		void go();
		void stop();
};

#endif