#include "C3SButton.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

void C3SButton::play() {
	isFirst = false;
	playbtn->setBright(false);
	playbtn->setTouchEnabled(false);
}
void C3SButton::replay() {
	isFirst = true;
	playbtn->setBright(true);
	playbtn->setTouchEnabled(true);
}