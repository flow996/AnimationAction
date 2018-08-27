#ifndef __ANIMATIONACTION_SCENE_H__
#define __ANIMATIONACTION_SCENE_H__

#define EXERCISE 0

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "Final\c321go.h"
#include "Final\CRunner.h"
#include "Final\CEnemy.h"
#include "Final\C3SButton.h"
#include "Final\CBean.h"
#include <stdio.h>
USING_NS_CC;

class AnimationAction : public cocos2d::Layer
{
private:
	cocos2d::Action   *_myAction;
	cocos2d::JumpBy   *_myJump;					// For Double Jump
	cocos2d::CallFunc *_mycallback;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int _NoJumps, damaged, which;	
	int blood = 100;
	float time = 0;
	float tot = 0;
	bool _bGamestart = 0;						//�C���O�_�}�l
	bool _bgo = 0;								//GO���}��
	bool hasEn, good,pointed,bad,bleed;			
	int  _sceneno;								//����
	char _cSceneNo[10];							//�����x�s
	unsigned int eid;							//�����ɫŧi(�歵)
	bool _btot;
	c321go *_go;
	CRunner *runner;
	CEnemy *enemy1,*enemy2, *enemy3;								//�ثe�u�ϥΤ@����ê��
	C3SButton button;
	CBean *bean;
	cocos2d::Label *label1;
	cocos2d::Point _runnerPt;
	cocos2d::Point _enemyPt;
	cocos2d::ui::Text *score;
	cocos2d::ui::Text *over;
	cocos2d::ui::LoadingBar *hp;

public:
	~AnimationAction();
// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	void doStep(float dt);
    virtual bool init();
// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

//cubebtn�BReplayBtn�Bcheck box-------------------------------------------------------------
	void CuberBtnTouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void ReplayBtnTouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void checkBoxTouchEvent(cocos2d::Ref* object, cocos2d::ui::CheckBox::EventType type);
	
	void EnemyRun(int en);									//���ƶ]����ê��
	void EnemyReset(int en);
	void AnimationAction::actionFinished();					// Action �� CallBack �禡
	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ� 

	// implement the "static create()" method manually
	CREATE_FUNC(AnimationAction);
};
#endif // __ANIMATIONACTION_SCENE_H__
