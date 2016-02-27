#pragma once

#include "cocos2d.h"
#include <map>

class WinScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float) override;
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	
	cocos2d::Label* menu_play;
	//cocos2d::Label* menu_help;
	cocos2d::Label* menu_exit;
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);

	void play();
	//void instructions();
	void exit();
    
    // implement the "static create()" method manually
    CREATE_FUNC(WinScene);
private:
	int winselect;
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
	cocos2d::Sprite* winselector;
};
