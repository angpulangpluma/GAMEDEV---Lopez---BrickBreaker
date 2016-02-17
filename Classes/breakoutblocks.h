#pragma once

#include "cocos2d.h"

class Breakout : public cocos2d::Layer
{
public:
	cocos2d::Sprite* ball;
	cocos2d::Sprite* platform;
	cocos2d::Vector<cocos2d::Sprite*> walls;
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Breakout);
};
