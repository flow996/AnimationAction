#include "CEnemy.h"

USING_NS_CC;

// 建立角色之前，必須先確定已經讀入儲存角色所有動態圖片的 plist 檔

CEnemy::CEnemy(const char *csbname, cocos2d::Layer &parent)	{
	_enemyRoot = CSLoader::createNode(csbname);				// 讀取角色
	_enemyRoot->setPosition(0,0);							// 預設放在 (0,0)
	_body = (cocos2d::Sprite *)_enemyRoot->getChildByName("triblock");
	_shadow = (cocos2d::Sprite *)_enemyRoot->getChildByName("trishadow_1");
	_fire1 = (cocos2d::Node *)_enemyRoot->getChildByName("fire11");
	_fire2 = (cocos2d::Node *)_enemyRoot->getChildByName("fire22");
	_fire1->setVisible(false);
	_fire2->setVisible(false);	//特效預設關閉
	_bDamage = false;			//一開始沒受傷
	getRect();
	_action = (ActionTimeline *)CSLoader::createTimeline(csbname);
	_action->retain();
	parent.addChild(_enemyRoot);
}
CEnemy::~CEnemy()	{
	_action->release();							//釋放被retain的動作
}
void CEnemy::getRect()	{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	size = _body->getContentSize();				//大小抓精靈
	_enemyPt = _enemyRoot->getPosition();		//位置抓節點
	rectenemy = Rect(_enemyPt.x, _enemyPt.y , size.width, size.height);
	/*_enemyRoot->setPhysicsBody(PhysicsBody::createBox(size));
	_enemyRoot->getPhysicsBody()->setDynamic(false);*/
	//雅媛說這兩行可以監看size？但還沒有成功過
}

bool CEnemy::detect(CEnemy &Ene, Point run)	{ //判斷碰撞
	Ene.getRect();
	if (Ene.rectenemy.containsPoint(run)){
		Ene._bDamage = true;
		return true;
	}
	else return false;
}

void CEnemy::go(int dif)	 { //三種不同的障礙物
	float r ,sp;		//r：移動時間	sp：動畫速度
	int x;				//x：移動距離
	if (dif == 0)	{			 //極快
		_fire1->setVisible(true);
		r = 1.8f;	x = 1440;	sp = 1.0f;
	}
	else if (dif == 1)	{		 //快
		_fire2->setVisible(true);
		r = 1.5f;	x = 1440;	sp = 1.0f;
	}
	else  {						 //慢
		_fire1->setVisible(false);
		_fire2->setVisible(false);
		r = 2.0f;	x = 1440;	sp = 1.0f;
	}
	MoveBy * moveby = MoveBy::create(r, Point(x, 0));
	_enemyRoot->runAction(Sequence::create(moveby, _mycallback, NULL));
	_enemyRoot->runAction(_action);
	_action->setTimeSpeed(sp);
	_action->gotoFrameAndPlay(0, 35, false);
}

void CEnemy::setPosition(const float x, const float y)
{
	_enemyRoot->setPosition(x, y);
}
