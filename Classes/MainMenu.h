#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);

	void play(Ref *pSender);
	void instructions(Ref *pSender);
	void exit(Ref *pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
};
