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

    virtual void onEnter() override;
	virtual void update(float) override;
	void restartGame();
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void onContactSeperate(cocos2d::PhysicsContact& contact);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Breakout);

private:
	cocos2d::PhysicsWorld *sceneWorld;

	void setPhysicsWorld(cocos2d::PhysicsWorld *world) { sceneWorld = world; };
};
