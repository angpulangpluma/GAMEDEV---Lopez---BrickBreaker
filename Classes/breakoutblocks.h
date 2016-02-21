#pragma once

#include "cocos2d.h"

class Breakout : public cocos2d::Layer
{
private:
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
public:
	cocos2d::Sprite* ball;
	cocos2d::Sprite* platform;
	cocos2d::Vector<cocos2d::Sprite*> walls;
    static cocos2d::Scene* createScene();
	bool start;

    virtual bool init();
	virtual void update(float) override;
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Breakout);
};
