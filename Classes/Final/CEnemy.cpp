#include "CEnemy.h"

USING_NS_CC;

// �إߨ��⤧�e�A�������T�w�w�gŪ�J�x�s����Ҧ��ʺA�Ϥ��� plist ��

CEnemy::CEnemy(const char *csbname, cocos2d::Layer &parent)	{
	_enemyRoot = CSLoader::createNode(csbname);				// Ū������
	_enemyRoot->setPosition(0,0);							// �w�]��b (0,0)
	_body = (cocos2d::Sprite *)_enemyRoot->getChildByName("triblock");
	_shadow = (cocos2d::Sprite *)_enemyRoot->getChildByName("trishadow_1");
	_fire1 = (cocos2d::Node *)_enemyRoot->getChildByName("fire11");
	_fire2 = (cocos2d::Node *)_enemyRoot->getChildByName("fire22");
	_fire1->setVisible(false);
	_fire2->setVisible(false);	//�S�Ĺw�]����
	_bDamage = false;			//�@�}�l�S����
	getRect();
	_action = (ActionTimeline *)CSLoader::createTimeline(csbname);
	_action->retain();
	parent.addChild(_enemyRoot);
}
CEnemy::~CEnemy()	{
	_action->release();							//����Qretain���ʧ@
}
void CEnemy::getRect()	{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	size = _body->getContentSize();				//�j�p����F
	_enemyPt = _enemyRoot->getPosition();		//��m��`�I
	rectenemy = Rect(_enemyPt.x, _enemyPt.y , size.width, size.height);
	/*_enemyRoot->setPhysicsBody(PhysicsBody::createBox(size));
	_enemyRoot->getPhysicsBody()->setDynamic(false);*/
	//���D���o���i�H�ʬ�size�H���٨S�����\�L
}

bool CEnemy::detect(CEnemy &Ene, Point run)	{ //�P�_�I��
	Ene.getRect();
	if (Ene.rectenemy.containsPoint(run)){
		Ene._bDamage = true;
		return true;
	}
	else return false;
}

void CEnemy::go(int dif)	 { //�T�ؤ��P����ê��
	float r ,sp;		//r�G���ʮɶ�	sp�G�ʵe�t��
	int x;				//x�G���ʶZ��
	if (dif == 0)	{			 //����
		_fire1->setVisible(true);
		r = 1.8f;	x = 1440;	sp = 1.0f;
	}
	else if (dif == 1)	{		 //��
		_fire2->setVisible(true);
		r = 1.5f;	x = 1440;	sp = 1.0f;
	}
	else  {						 //�C
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
