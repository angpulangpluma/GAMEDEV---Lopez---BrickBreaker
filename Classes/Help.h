#pragma once

#include "cocos2d.h"
#include <map>

class Help : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float) override;
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
    
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Help);

private:
	int sequence;
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
	cocos2d::Label * help_desc;
	cocos2d::Sprite * help_pic;
};
