#ifndef __ANIMATIONACTION_SCENE_H__
#define __ANIMATIONACTION_SCENE_H__

#define EXERCISE 0

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"

class AnimationAction : public cocos2d::Layer
{
private:
	cocos2d::Sequence *_mySequence;
	cocos2d::Action   *_myAction;

	// For Double Jump
	cocos2d::JumpBy   *_myJump;
	int _NoJumps;
	cocos2d::CallFunc *_mycallback;
	cocos2d::Point _runnerPt;

	// �Ұ�m�ߤ@
#if EXERCISE == 1
	cocos2d::Sprite *_runner;
	cocos2d::JumpTo *_jumpAction;

#elif EXERCISE == 2
	// loadingBar 
	cocos2d::ui::LoadingBar *_loadingBar;
#endif

	cocos2d::ui::Text *_sliderValue;

	// Ū�J�P����զX�����ǦC�V�ʵe
	cocos2d::Node *_triRoot2;
	cocostudio::timeline::ActionTimeline *_triAction;


public:
	~AnimationAction();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void doStep(float dt);
	
	//cubebtn
	void CuberBtnTouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//check box & slider
	void checkBoxTouchEvent(cocos2d::Ref* object, cocos2d::ui::CheckBox::EventType type);
	//void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);

	// Action �� CallBack �禡
	void AnimationAction::actionFinished();

	//Ĳ�I
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ� 

	// implement the "static create()" method manually
    CREATE_FUNC(AnimationAction);
};

#endif // __ANIMATIONACTION_SCENE_H__
