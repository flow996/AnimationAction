#include "AnimationAction.h"
#include "cocostudio/CocoStudio.h"
#include "Final\CRunner.h"

USING_NS_CC;

#define DOUBLEJUMP 1

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* AnimationAction::createScene()		{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = AnimationAction::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

AnimationAction::~AnimationAction()		{
	if( _myAction != nullptr ) _myAction->release();
	if (_myJump != nullptr) _myJump->release();
	if (_mycallback != nullptr) _mycallback->release();
	
	if (_go != nullptr) _go->release();
	if (enemy1 != nullptr) enemy1->release();
	if (enemy2 != nullptr) enemy2->release();
	AnimationCache::destroyInstance();  // ���� AnimationCache ���o���귽
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool AnimationAction::init()
{
	_myAction = nullptr;
	_myJump = nullptr;
	_mycallback = nullptr;

    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )    {
        return false;
    }
    auto rootNode = CSLoader::createNode("AnimationAction.csb");
    addChild(rootNode);
//	Ū�J�x�s�h�i�Ϥ��� plist ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene101.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("321go.plist");
	Size size;
//	����C321Go----------------------------------------------------------------------------------
	_go = new c321go();
	_go->init(Point(visibleSize.width / 2.0f, visibleSize.height / 2.0f), *this);

// ���ĻP���� ----------------------------------------------------------------------------------
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/SR_bg.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);			//�|����@
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();					// ����I������
	SimpleAudioEngine::getInstance()->preloadEffect("thinking cloud.mp3");		// �w�����J������
	//unsigned int eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", true);  // ���񭵮���
	//SimpleAudioEngine::getInstance()->stopEffect(eid);// ����ļ���A�����ϥ� PlayEffect �Ǧ^�� id
	//SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");		// ���񭵮���

//	Cuberbtn------------------------------------------------------------------------------------																		  // Button -------------------------------------------------------------------------------------
	button.playbtn = dynamic_cast<Button*>(rootNode->getChildByName("CuberBtn"));
	//Button *cuberbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("CuberBtn"));
	button.playbtn->addTouchEventListener(CC_CALLBACK_2(AnimationAction::CuberBtnTouchEvent, this));	

//	Replaybtn-----------------------------------------------------------------------------------														  // Button -------------------------------------------------------------------------------------
	button.replaybtn = dynamic_cast<Button*>(rootNode->getChildByName("ReplayBtn"));
	//Button *replaybtn = (cocos2d::ui::Button *)(rootNode->getChildByName("ReplayBtn"));
	button.replaybtn->addTouchEventListener(CC_CALLBACK_2(AnimationAction::ReplayBtnTouchEvent, this));

//	CheckBox------------------------------------------------------------------------------------
	CheckBox *checkBox = dynamic_cast<CheckBox*>(rootNode->getChildByName("CheckBox_1"));
	//	CheckBox *checkBox = (cocos2d::ui::CheckBox*)(rootNode->getChildByName("CheckBox_1"));
	checkBox->addEventListener(CC_CALLBACK_2(AnimationAction::checkBoxTouchEvent, this));

//	����----------------------------------------------------------------------------------------
	label1 = Label::createWithBMFont("fonts/couriernew32.fnt", _cSceneNo);
	size = label1->getContentSize();
	label1->setColor(Color3B::WHITE);
	label1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - size.height));
	this->addChild(label1, 1);
	strcpy(this->_cSceneNo, "SCORE 000");
	label1->setString(_cSceneNo);
	over = (cocos2d::ui::Text*)rootNode->getChildByName("gameover");
	over->setVisible(false);
//	loadingBar ---------------------------------------------------------------------------------
	hp = (cocos2d::ui::LoadingBar *)rootNode->getChildByName("hp");
	hp->setDirection(LoadingBar::Direction::LEFT);
	hp->setPercent(blood);

//  RunnerNode----------------------------------------------------------------------------------
	runner = new CRunner("RunnerNode.csb", *this);
	runner->setPosition(visibleSize.width / 2.0f + 400, visibleSize.height / 2.0f);
	runner->setAnimation("cuberanim.plist");
	runner->stop();
	runner->_runnerRoot->setTag(101);
	_NoJumps = 0;
	_myJump = JumpBy::create(0.65f, Point(0, 0), 150, 1);
	_myJump->retain();
	runner->getRect();
	_runnerPt = runner->_runnerRoot->getPosition();
//----------------------------------------------------------------------------------------------
	_mycallback = CallFunc::create(this, callfunc_selector(AnimationAction::actionFinished));
	_mycallback->retain();
//	Enemy---------------------------------------------------------------------------------------
	enemy1= new CEnemy("triangleNode.csb", *this);
	enemy1->setPosition(visibleSize.width / 2.0f - 720, visibleSize.height / 2.0f);
	_enemyPt = enemy1->_enemyRoot->getPosition();

	enemy2 = new CEnemy("triangleNode.csb", *this);
	enemy2->setPosition(visibleSize.width / 2.0f - 720, visibleSize.height / 2.0f);
	_enemyPt = enemy2->_enemyRoot->getPosition();
	
	enemy3 = new CEnemy("triangleNode.csb", *this);
	enemy3->setPosition(visibleSize.width / 2.0f - 720, visibleSize.height / 2.0f);
	_enemyPt = enemy3->_enemyRoot->getPosition();
//----------------------------------------------------------------------------------------------
	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(AnimationAction::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(AnimationAction::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(AnimationAction::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�
	
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(AnimationAction::doStep));
    return true;
}

void AnimationAction::actionFinished()		{// do something on complete
	CCLOG("B %d\n", _NoJumps);				
	_NoJumps--;
	if (_NoJumps == 1) {
		auto moveto = MoveTo::create(0.15f, _runnerPt);
		auto actionBody = this->getChildByTag(101);
		actionBody->runAction(moveto);
		_NoJumps--;
	}
}

void AnimationAction::EnemyRun(int en) {
	if (en ==0)	{
		enemy1->_bDamage = false;					//���s�P�_�O�_����
		enemy1->go(en);								//�]�B
		hasEn = true;								//�ثe���Ǫ�
	}
	else if (en == 1) {
		enemy2->_bDamage = false;					//���s�P�_�O�_����
		enemy2->go(en);								//�]�B
		hasEn = true;								//�ثe���Ǫ�
	}
	else {
		enemy3->_bDamage = false;					//���s�P�_�O�_����
		enemy3->go(en);								//�]�B
		hasEn = true;								//�ثe���Ǫ�
	}

}
void AnimationAction::EnemyReset(int en) {			//�]���᭫�]��m
	if (en ==0)	{
		enemy1->setPosition(visibleSize.width / 2.0f - 720, visibleSize.height / 2.0f);
	}
	else if (en == 1) {
		enemy2->setPosition(visibleSize.width / 2.0f - 720, visibleSize.height / 2.0f);
	}
	else {
		enemy3->setPosition(visibleSize.width / 2.0f - 720, visibleSize.height / 2.0f);
	}
}
void AnimationAction::doStep(float dt)		{
	time += dt;
	if (_bgo)	{
		if (_go->doStep(dt))	{
			_bgo = false;
			_bGamestart = true;
			runner->go();
			button.play();
		};
	}
	if (_bGamestart)	{		//�C���}�l��
		runner->getRect();		//�P�_���
		enemy1->getRect();		//�P�_�T��
		enemy2->getRect();		//�P�_�T��
		enemy3->getRect();		//�P�_�T��
		bool boom = enemy1->detect(*enemy1, runner->_runnerPt);	//�P�_�I��
		bool boom1 = enemy2->detect(*enemy2, runner->_runnerPt);	//�P�_�I��
		bool boom2 = enemy3->detect(*enemy3, runner->_runnerPt);	//�P�_�I��
		
		if (time>=5.0f)		{	//�C5��N���T���]�@��		
			which = random(0, 2);
			EnemyRun(which);
			time -= 5.0f; hasEn = true;
		}
		if (hasEn)		{
			if (boom)	{									//�p�G1����F
				_btot = true; bad = true; bleed = true; which = 0;
				if ((enemy1->_enemyPt.x) - 319 >= runner->_runnerPt.x) {				
					 EnemyReset(0);hasEn = false;
				}
			}
			else if ((enemy1->_enemyPt.x) - 320 >=runner->_runnerPt.x) {//�p�G1�S����				
				_btot = true; good = true; which = 0; pointed = true; EnemyReset(0); hasEn = false;
			}
			else if (boom1) {									//�p�G2����F
				_btot = true; bad = true; bleed = true; which = 1;
				if ((enemy2->_enemyPt.x) - 319 >= runner->_runnerPt.x) {				
					EnemyReset(1); hasEn = false;
				}
			}
			else if ((enemy2->_enemyPt.x) - 320 >= runner->_runnerPt.x) {//�p�G2�S����				
				_btot = true; good = true; which = 1; pointed = true; EnemyReset(1); hasEn = false;
			}
			else if (boom2) {									//�p�G3����F
				_btot = true; bad = true; bleed = true; which = 2;
				if ((enemy3->_enemyPt.x) - 319 >= runner->_runnerPt.x) {			
					EnemyReset(2); hasEn = false;
				}
			}
			else if ((enemy3->_enemyPt.x) - 320 >= runner->_runnerPt.x) {//�p�G3�S����				
				_btot = true; good = true; which = 2; pointed = true; EnemyReset(2); hasEn = false;
			}	
		} 
		if (_btot)		{
			tot += dt;
			if (good)		{
				runner->_body->setColor(Color3B(75, 155, 77));
				runner->_happyFace->setVisible(true);
				if (pointed)	{
					if (which==0)			{
						/*this->_sceneno++;
						int i = this->_sceneno, j = 0;
						while (i > 0) {
							this->_cSceneNo[7 - j] = i % 10 + 48;
							i = i / 10;
							j++;
						}
						label1->setString(_cSceneNo);
						pointed = false;*/
					}
					else if (which ==1)		{
						/*this->_sceneno++;
						int i = this->_sceneno, j = 0;
						while (i > 0) {
							this->_cSceneNo[8 - j] = i % 10 + 48;
							i = i / 10;
							j++;
						}
						score->setString(_cSceneNo);
						pointed = false;*/
					}
					else	{
						/*this->_sceneno++;
						int i = this->_sceneno, j = 0;
						while (i > 0) {
							this->_cSceneNo[8 - j] = i % 10 + 48;
							i = i / 10;
							j++;
						}
						score->setString(_cSceneNo);
						pointed = false;*/
					}
				}
			}
			if (bad)		{
				runner->_body->setColor(Color3B(209, 73, 63));
				runner->_depressedFace->setVisible(true);
				if (bleed)				{
					if (which == 0) {
						blood -= 5;
						hp->setPercent(blood);
						bleed = false;
					}
					else if (which == 1) {				
						blood -=10;
						hp->setPercent(blood);
						bleed = false;
					}
					else {						
						blood -= 20;
						hp->setPercent(blood);
						bleed = false;
					}
				}
			}
			if (tot >= 2.0f) {
				good = false; bad = false;
				runner->_body->setColor(Color3B(255, 255, 255));
				runner->_normalFace->setVisible(true);
				runner->_happyFace->setVisible(false);
				runner->_depressedFace->setVisible(false);
				tot -= 2.0f;
				_btot = false;
			}
		}
		if (blood <=0)		{
			blood = 0;
			hp->setPercent(blood);
			runner->_body->setColor(Color3B(209, 73, 63));
			runner->_depressedFace->setVisible(true);
			over->setVisible(true);
			_bGamestart = false;
		}
	}

}

void AnimationAction::CuberBtnTouchEvent(Ref *pSender, Widget::TouchEventType type)		{
	switch (type)	{
		case Widget::TouchEventType::BEGAN:
			CCLOG("Touch Down");
			if (!_bgo && !_bGamestart)_bgo = true;
			strcpy(this->_cSceneNo, "SCORE 000");
			label1->setString(_cSceneNo);
			over->setVisible(false);
			break;
		case Widget::TouchEventType::MOVED:
			CCLOG("Touch Move");
			break;
		case Widget::TouchEventType::ENDED:
			CCLOG("Touch Up");
			break;
		case Widget::TouchEventType::CANCELED:
			CCLOG("Touch Cancelled");
			break;
		default:
			break;
	}
}
//���bottom

void AnimationAction::ReplayBtnTouchEvent(Ref *pSender, Widget::TouchEventType type)	{
	switch (type)	{
	case Widget::TouchEventType::BEGAN:
		CCLOG("Touch Down");
		_go->reset(Point(visibleSize.width / 2.0f, visibleSize.height / 2.0f), *this);
		_bgo = false;
		_bGamestart = false;
		runner->_body->setColor(Color3B(255, 255, 255));
		runner->_normalFace->setVisible(true);
		runner->_happyFace->setVisible(false);
		runner->_depressedFace->setVisible(false);
		runner->stop();
		button.replay();
		/*strcpy(this->_cSceneNo, "SCORE : 000");
		score->setString(_cSceneNo);*/
		blood = 100;
		hp->setPercent(blood);
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("Touch Move");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("Touch Up");
		break;
	case Widget::TouchEventType::CANCELED:
		CCLOG("Touch Cancelled");
		break;
	default:
		break;
	};
}
//���bottom

bool AnimationAction::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)		{//Ĳ�I�}�l�ƥ�
	Point touchLoc = pTouch->getLocation();
	auto actionBody = this->getChildByTag(101);
	if (_bGamestart && _NoJumps == 0)	{
		actionBody->runAction(Sequence::create(_myJump, _mycallback, NULL));
		_NoJumps++;
		eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", false);
		//runner->_shadow->setPosition(visibleSize.width / 2.0f + 400, visibleSize.height / 2.0f);
		//runner->_shadow->setScale(1 + t);
	}
	else if (_NoJumps == 1) {  // �ĤG���Q���U
		_NoJumps++;
		actionBody->stopAllActions();
		actionBody->runAction(Sequence::create(_myJump, _mycallback, NULL));
		eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", false);
		//runner->_shadow->setPosition(visibleSize.width / 2.0f + 400, visibleSize.height / 2.0f);
		//runner->_shadow->setScale(1 + t);
	}
	return true;
}
//Ĳ�I�}�l�ƥ�

void  AnimationAction::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {//Ĳ�I���ʨƥ�
	Point touchLoc = pTouch->getLocation();
}
//Ĳ�I���ʨƥ�

void  AnimationAction::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
	Point touchLoc = pTouch->getLocation();
	if (_bGamestart)	{
		
	}
}
//Ĳ�I�����ƥ� 

void AnimationAction::checkBoxTouchEvent(Ref* object, CheckBox::EventType type) {
	switch (type)	{
		case CheckBox::EventType::SELECTED:
			CCLOG("Selected");
			unsigned int eid ;  
			SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/SR_bg.mp3", true);
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //�|����@
			//SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// ����I������
			SimpleAudioEngine::getInstance()->preloadEffect("thinking cloud.mp3");	// �w�����J������
	//		eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", true);  // ���񭵮���	
			//SimpleAudioEngine::getInstance()->stopEffect(eid);  // ����ļ���A�����ϥ� PlayEffect �Ǧ^�� id
			//SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");  // ���񭵮���
		break;
		case CheckBox::EventType::UNSELECTED:
			CCLOG("Unselected");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();// ����I������
//			SimpleAudioEngine::getInstance()->stopEffect(eid); //����ļ���A�����ϥ� PlayEffect �Ǧ^�� id
			SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");  // ���񭵮���
		break;
	}
}
//check box ���Ķ}��