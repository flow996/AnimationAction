#include "AnimationAction.h"
#include "cocostudio/CocoStudio.h"
#include "Final\CRunner.h"

USING_NS_CC;

//#define DOUBLEJUMP 1

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* AnimationAction::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = AnimationAction::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

AnimationAction::~AnimationAction()
{
	if( _myAction != nullptr ) _myAction->release();
	if (_myJump != nullptr) _myJump->release();
	if (_mycallback != nullptr) _mycallback->release();

	AnimationCache::destroyInstance();  // 釋放 AnimationCache 取得的資源
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool AnimationAction::init()
{
	_myAction = nullptr;
	_myJump = nullptr;
	_mycallback = nullptr;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("AnimationAction.csb");
    addChild(rootNode);

// 音效與音樂 --------------------------------------------------------------------------------
	//	auto bkmusic = (cocostudio::ComAudio *)rootNode->getChildByName("bkmusic")->getComponent("bkmusic");
	//	bkmusic->playBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/SR_bg.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //尚未實作
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// 停止背景音樂
	SimpleAudioEngine::getInstance()->preloadEffect("thinking cloud.mp3");	// 預先載入音效檔
	unsigned int eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", true);  // 播放音效檔
	//SimpleAudioEngine::getInstance()->stopEffect(eid);  // 停止音效撥放，必須使用 PlayEffect 傳回的 id
	//SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");  // 釋放音效檔
//---------------------------------------------------------------------------------------------

// Button -------------------------------------------------------------------------------------
	Button *cuberbtn = dynamic_cast<Button*>(rootNode->getChildByName("CuberBtn"));
	//Button *cuberbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("CuberBtn"));
	cuberbtn->addTouchEventListener(CC_CALLBACK_2(AnimationAction::CuberBtnTouchEvent, this));
//---------------------------------------------------------------------------------------------	

// 文本 TTF 字型-------------------------------------------------------------------------------
	//auto text = (cocos2d::ui::Text *)rootNode->getChildByName("Text101");
	//text->setColor(Color3B(200, 255, 50));

	//以下用於 slider 顯示用
	//_sliderValue = (cocos2d::ui::Text *)rootNode->getChildByName("Text101");
//---------------------------------------------------------------------------------------------	

// CheckBox-------------------------------------------------------------------------------
	CheckBox *checkBox = dynamic_cast<CheckBox*>(rootNode->getChildByName("CheckBox_1"));
	//	CheckBox *checkBox = (cocos2d::ui::CheckBox*)(rootNode->getChildByName("CheckBox_1"));
	checkBox->addEventListener(CC_CALLBACK_2(AnimationAction::checkBoxTouchEvent, this));
//---------------------------------------------------------------------------------------------	

// loadingBar -------------------------------------------------------------------------------
	#if EXERCISE != 2
		auto loadingBar = (cocos2d::ui::LoadingBar *)rootNode->getChildByName("LoadingBar_1");
		loadingBar->setDirection(LoadingBar::Direction::LEFT);
		loadingBar->setPercent(100);
	#else //課堂練習二
		_loadingBar = (cocos2d::ui::LoadingBar *)rootNode->getChildByName("LoadingBar_1");
		_loadingBar->setDirection(LoadingBar::Direction::LEFT);
		_loadingBar->setPercent(100);
	#endif
//---------------------------------------------------------------------------------------------	

// Slider 滑動條 -------------------------------------------------------------------------------------
		//auto *slider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_1"));
		//slider->addEventListener(CC_CALLBACK_2(AnimationAction::sliderEvent, this));
		//slider->setPercent(0);
		//slider->setMaxPercent(100);		// 可以設定超過 100
//---------------------------------------------------------------------------------------------	


// 利用程式直接產生序列幀動畫 
// STEP 1 : 讀入儲存多張圖片的 plist 檔
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene101.plist");
// ------------------------------------------------------------------------------------------------- 

// 方法一  利用 SpriteFrameCache 來儲存每一張要連續撥放的圖片

	// STEP 2 : 產生連續圖片的檔名並加入 animation 中
	auto frameCache = SpriteFrameCache::getInstance();
	// 利用 STL 的 vector 來儲存連續的 SpriteFrame
	Vector<SpriteFrame*> animFrames(8);
	char tmp[50];
	for (int j = 1; j <= 8; j++)
	{
		sprintf(tmp, "cuber%02d.png", j); // 產生 cuber01.png ~  cuber08.png
										  //從 plist 中取得圖檔名稱並建立 spriteFrame
		auto frame = frameCache->getSpriteFrameByName(tmp);
		animFrames.pushBack(frame);
	}
	// STEP 3: 建立 animation
	auto animation1 = Animation::createWithSpriteFrames(animFrames, 0.05f);

	// STEP 4：建立序列幀動畫的主體
	auto runner1 = Sprite::createWithSpriteFrameName("cuber01.png");
	runner1->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f + 150);
	this->addChild(runner1);

	// STEP 5：由主體呼叫動畫的播放
	runner1->runAction(RepeatForever::create(Animate::create(animation1)));// 一直重複播放

//--------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------- 
// 方法二 利用儲存序列幀動畫的 plist 來建立
/*
	// STEP 2: 以 AnimationCache 來讀取該 plist 檔
	auto animCache = AnimationCache::getInstance();
	animCache->addAnimationsWithFile("cuberanim.plist");
	auto animation2 = animCache->getAnimation("running");

	// STEP 3: 建立 animation
	auto action2 = RepeatForever::create(Animate::create(animation2));

	// STEP 4：建立序列幀動畫的主體
	auto runner2 = Sprite::createWithSpriteFrameName("cuber01.png");
	runner2->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f-150);
	this->addChild(runner2);

	// STEP 5：由主體呼叫動畫的播放
	runner2->runAction(action2);// 一直重複播放
*/
//--------------------------------------------------------------------------------------------------


// Action -----------------------------------------------------------------------------------------
	auto actionBody = Sprite::createWithSpriteFrameName("cuber01.png");
	actionBody->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	actionBody->setTag(101);	// 用於取得該物件
	actionBody->setColor(Color3B(200, 209, 63));
	this->addChild(actionBody);
	// 後續其他的範例都使用這個 actionBody 

// BezierBy/BezierTo
	// BezierTo
	ccBezierConfig bezier;
	/*
	bezier.controlPoint_1 = Point(540, 720);
	bezier.controlPoint_2 = Point(140, 720);
	bezier.endPosition = Point(40, 360);
	BezierTo * bezierto = BezierTo::create(1.5f, bezier);
	actionBody->runAction(bezierto);
	*/
	// BezierBy
	//	bezier.endPosition = Point(0, visibleSize.height / 2.0f);
	/*
	bezier.controlPoint_1 = Point(-100, 360);
	bezier.controlPoint_2 = Point(-500, 360);
	bezier.endPosition = Point(-600, 0);
	BezierBy * bezierby = BezierBy::create(1.5f, bezier);
	actionBody->runAction(bezierby);
	// 產生反向的 Action
	auto bezierbyBack = bezierby->reverse();
	// 將不同的 Action 組成連續撥放的 Action 組合
	auto sequence = cocos2d::Sequence::create(bezierby, bezierbyBack, NULL);
	// 執行這個 Action 執行序列的組合
	actionBody->runAction(sequence);
	*/
//-------------------------------------------------------------------------------------------------

// MoveTo/MoveBy
	/*
	MoveTo * moveto = MoveTo::create(1.25f, Point(40, 360));
	actionBody->runAction(moveto);
	// 產生反向的 Action，上一行以及以下的三行擇一直行即可
	MoveBy * moveby = MoveBy::create(1.25f, Point(-600, 0));	
	auto movebyBack = moveby->reverse();
	actionBody->runAction(Sequence::create(moveby, movebyBack, NULL));
	*/
//-------------------------------------------------------------------------------------------------

// JumpTo/JumpBy
/*
	JumpTo * jumpto = JumpTo::create(1.25f, Point(40, 360), 150, 3);
	actionBody->runAction(jumpto);
	// 產生反向的 Action，上一行以及以下的三行擇一直行即可
	JumpBy * jumpby = JumpBy::create(1.25f, Point(-600, 0), 150, 3);
	auto jumpbyBack = jumpby->reverse();
	auto sequence = Sequence::create(jumpby, jumpbyBack, NULL);
	actionBody->runAction(Sequence::create(jumpby, jumpbyBack, NULL));
*/
//-------------------------------------------------------------------------------------------------

// ScaleTo/ScaleTo
	/*
	ScaleTo * scaleto = ScaleTo::create(1.25f, 0.5f);
//	actionBody->runAction(scaleto);
	// 產生反向的 Action，上一行以及以下的所有程式碼擇一直行即可
	ScaleBy * scaleby = ScaleBy::create(1.25f, 0.5f);
	auto scalebyBack = scaleby->reverse();
	auto sequence = Sequence::create(scaleby, scalebyBack, NULL);
	actionBody->runAction(sequence);
	*/
//-------------------------------------------------------------------------------------------------

// FadeIn/FadeOut
/*
	FadeIn * fadein = FadeIn::create(1.25f);
	//actionBody->setOpacity(0);  // 單獨使用 FadeIn 必須先設定透明度為 0
//	actionBody->runAction(fadein);
	FadeOut * fadeout = FadeOut::create(1.25f);
//	actionBody->runAction(fadeout);
	FadeTo * fadeto = FadeTo::create(1.25f, 128);
	auto fadeoutBack = fadeout->reverse();
	auto sequence = Sequence::create(fadeout, fadein, fadeto, NULL);
	actionBody->runAction(sequence);
*/
//-------------------------------------------------------------------------------------------------

// TintTo/TintBy
/*
	TintTo * tintto = TintTo::create(1.0f, Color3B(82, 131, 151));
	actionBody->runAction(tintto);
	TintBy * tintby = TintBy::create(1.0f,-118, -78, 88);
//	actionBody->runAction(tintby);
	//auto tintbyBack = tintby->reverse();
	//auto sequence = Sequence::create(tintby, tintbyBack, NULL);
	//actionBody->runAction(sequence);
*/
//-------------------------------------------------------------------------------------------------

// RotateTo/RotateBy
	RotateTo * rotateto = RotateTo::create(1.0f, 60.0f);
//	actionBody->runAction(rotateto);	

	RotateBy * rotateby = RotateBy::create(1.0f, 30.0f);
	//actionBody->runAction(rotateby);
	//	旋轉 RotateBy 的累加效果   ---------------------------------
	//_myAction = (cocos2d::Action *)RotateBy::create(1.0f, 30.0f);
	//_myAction->retain();
	//---------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

// 多重 Action 的使用
	//JumpBy * jumpby = JumpBy::create(1.25f, Point(-600, 0), 150, 3);
	//auto jumpbyBack = jumpby->reverse();
	//auto sequence = Sequence::create(jumpby, jumpbyBack, NULL);
	//actionBody->runAction(Sequence::create(jumpby, jumpbyBack, NULL));

	//ScaleBy * scaleby = ScaleBy::create(1.25f, 0.5f);
	//auto scalebyBack = scaleby->reverse();
	//sequence = Sequence::create(scaleby, scalebyBack, NULL);
	//actionBody->runAction(sequence);

	// 使用 Spawn
	//auto mySpawn = Spawn::createWithTwoActions(jumpby, scaleby);
	//actionBody->runAction(mySpawn);
//-------------------------------------------------------------------------------------------------

//  Spawn 用在 Sequence 中
	//JumpBy * jumpby = JumpBy::create(1.25f, Point(-600, 0), 150, 3);
	//auto jumpbyBack = jumpby->reverse();
	//TintTo * tintto = TintTo::create(1.0f, Color3B(82, 131, 151));
	//auto mySpawn = Spawn::createWithTwoActions(jumpby, tintto);
	//auto sequence = Sequence::create(mySpawn, jumpbyBack, NULL);
	//actionBody->runAction(sequence);
//-------------------------------------------------------------------------------------------------

//  EaseSineOut 與 EaseSineIn 搭配 Action 的使用
	MoveBy * moveby = MoveBy::create(1.0f, Point(-600, 0));
	auto movebyBack = moveby->reverse();
	auto moveSineOut = EaseSineOut::create(moveby);
	auto delay = DelayTime::create(0.125f);
	auto moveSineIn = EaseSineIn::create(movebyBack);
	actionBody->runAction(Sequence::create(moveSineOut, delay, moveSineIn, NULL));
//-------------------------------------------------------------------------------------------------

//  EaseOut 與 EaseIn 搭配 Action 的使用
	//MoveBy * moveby = MoveBy::create(1.0f, Point(-600, 0));
	//auto movebyBack = moveby->reverse();
	//auto moveSineOut = EaseOut::create(moveby,3);
	//auto delay = DelayTime::create(0.125f);
	//auto moveSineIn = EaseIn::create(movebyBack, 3);
	//actionBody->runAction(Sequence::create(moveSineOut, delay, moveSineIn, NULL));
//-------------------------------------------------------------------------------------------------

//  Action 與 CallBack Function的使用
	/*MoveBy * moveby = MoveBy::create(1.0f, Point(-600, 0));
	auto moveSineOut = EaseOut::create(moveby,3);
	auto callback = CallFunc::create(this, callfunc_selector(AnimationAction::actionFinished));
	actionBody->runAction(Sequence::create(moveSineOut, callback, NULL));*/
//-------------------------------------------------------------------------------------------------

// Double Jump 
#ifdef DOUBLEJUMP
	_NoJumps = 0;
	_myJump = JumpBy::create(0.65f, Point(0, 0), 150, 1);
	_myJump->retain();
	actionBody->setScale(0.65f);
	actionBody->setPosition(visibleSize.width/2.0f, visibleSize.height / 2.0f-50);
	_runnerPt = actionBody->getPosition();
	_mycallback = CallFunc::create(this, callfunc_selector(AnimationAction::actionFinished));
	_mycallback->retain();
#endif
//-------------------------------------------------------------------------------------------------

// CSB 與程式建立序列幀動畫的組合
// Example 1 : ASimpleRunner 
	/*
	// 讀入 csb 檔並取得 runnerRoot
	auto runnerRoot = CSLoader::createNode("ASimpleRunner.csb");
	// 設定 runnerRoot 的位置，並加入場景中
	runnerRoot->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
	Point pt = runnerRoot->getPosition();
	runnerRoot->setScale(0.70f);
	this->addChild(runnerRoot);

	// 從 runnderNode 取得 Cuber01_1 並建立 Sprite
	auto runnerBody = (cocos2d::Sprite *)runnerRoot->getChildByName("cuber01_1");
	// 設定 runner 的顏色
	runnerBody->setColor(Color3B(255, 200, 50));

	// 建立 runnerBody 所需的序列幀動畫
	auto animCache1 = AnimationCache::getInstance();
	animCache1->addAnimationsWithFile("cuberanim.plist");
	auto animation3 = animCache1->getAnimation("running");
	auto action3 = RepeatForever::create(Animate::create(animation3));

	// 建立 runnerRoot 所需的 jumpAction
	auto jumpAction = cocos2d::JumpBy::create(1.25f, Point(-500 , 0), 100, 3);
	auto jumpBack = jumpAction->reverse();
	//以 runnerBody 為主體去執行方法一所建立的 Animation
	runnerBody->runAction(action3);
	//以 runnerRoot 同步執行 jumpAction
//	runnerRoot->runAction(cocos2d::Sequence::create(jumpAction, jumpBack, NULL));
 */
//-------------------------------------------------------------------------------------------------

// Example 2 : RunnerNode
	auto runner = CRunner("RunnerNode.csb", *this);
	runner.setPosition(visibleSize.width/ 2.0f + 300, visibleSize.height / 2.0f);
	runner.setAnimation("cuberanim.plist");
	runner.go();
//-------------------------------------------------------------------------------------------------
	
	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(AnimationAction::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(AnimationAction::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(AnimationAction::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(AnimationAction::doStep));
    return true;
}

void AnimationAction::actionFinished()
{
	// do something on complete
	CCLOG("B %d\n", _NoJumps);

#ifdef DOUBLEJUMP
	_NoJumps--;
	if (_NoJumps == 1) {
		auto moveto = MoveTo::create(0.15f, _runnerPt);
		auto actionBody = this->getChildByTag(101);
		actionBody->runAction(moveto);
		_NoJumps--;
	}
#endif

}


void AnimationAction::doStep(float dt)
{
	//#if EXERCISE == 2 //課堂練習二
	//	float t = _loadingBar->getPercent();
	//	if (t >= dt * 5)  t = t - dt * 5;
	//	else t = 0;
	//	_loadingBar->setPercent(t);
	//#else

	//#endif
}

void AnimationAction::CuberBtnTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
		case Widget::TouchEventType::BEGAN:
			log("Touch Down");
			break;
		case Widget::TouchEventType::MOVED:
			log("Touch Move");
			break;
		case Widget::TouchEventType::ENDED:
			log("Touch Up");
			break;
		case Widget::TouchEventType::CANCELED:
			log("Touch Cancelled");
			break;
		default:
			break;
	}
}
//方塊bottom

//void AnimationAction::sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
//{
//	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
//	{
//		Slider* slider = dynamic_cast<Slider*>(sender);
//		int percent = slider->getPercent();
//		int maxPercent = slider->getMaxPercent();
//		_sliderValue->setString(StringUtils::format("Percent %5.2f", 100.0 * percent / maxPercent));
//	}
//}
//滑動條

bool AnimationAction::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();

#ifdef DOUBLEJUMP
	auto actionBody = this->getChildByTag(101);
	if (_NoJumps == 0) {  // 第一次被按下
		actionBody->runAction(Sequence::create(_myJump, _mycallback, NULL));
		_NoJumps++;
	}
	else if(_NoJumps == 1 ){  // 第二次被按下
		_NoJumps++;
		actionBody->stopAllActions();
		actionBody->runAction(Sequence::create(_myJump, _mycallback, NULL));
	}
#endif
	return true;
}
//觸碰開始事件

void  AnimationAction::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();

}
//觸碰移動事件

void  AnimationAction::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) 
{
	Point touchLoc = pTouch->getLocation();

//	旋轉 RotateBy 的累加效果   ---------------------------------
	//auto actionbody = this->getChildByTag(101);
	//actionbody->runAction(_myAction);
	//CCLOG("%d", actionbody->getNumberOfRunningActions());
//--------------------------------------------------------------
}

//觸碰結束事件 

void AnimationAction::checkBoxTouchEvent(Ref* object, CheckBox::EventType type) {
	switch (type)
	{
		case CheckBox::EventType::SELECTED:
			log("Selected");
			unsigned int eid;
			SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/SR_bg.mp3", true);
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);  //尚未實作
			//SimpleAudioEngine::getInstance()->stopBackgroundMusic();	// 停止背景音樂
			SimpleAudioEngine::getInstance()->preloadEffect("thinking cloud.mp3");	// 預先載入音效檔
			eid = SimpleAudioEngine::getInstance()->playEffect("thinking cloud.mp3", true);  // 播放音效檔																				 //SimpleAudioEngine::getInstance()->stopEffect(eid);  // 停止音效撥放，必須使用 PlayEffect 傳回的 id
			//SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");  // 釋放音效檔
		break;

		case CheckBox::EventType::UNSELECTED:
			log("Unselected");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();// 停止背景音樂
			SimpleAudioEngine::getInstance()->stopEffect(eid); //停止音效撥放，必須使用 PlayEffect 傳回的 id
			SimpleAudioEngine::getInstance()->unloadEffect("thinking cloud.mp3");  // 釋放音效檔
		break;
	}
}
//check box 音效開關