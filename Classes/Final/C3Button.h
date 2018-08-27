#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "cocostudio/CocoStudio.h"
class C3SButton:public cocos2d::ui::Button
{
public:
	//C3SButton();
	//~C3SButton();
	cocos2d::ui::Button *playbtn,*replaybtn;
	int timer=0;
	void play();
	bool isFirst = true;
	void replay();
	cocos2d::ui::Text *playtxt;
private:
	
};
