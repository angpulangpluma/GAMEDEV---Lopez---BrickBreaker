#include "Story.h"

USING_NS_CC;

Scene* Story::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Story::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Story::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto label = Label::createWithSystemFont("Hello World", "Arial", 96);
	label->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
	label->setPosition(50, 350);
	//label->setPosition(500, 500);
	this->addChild(label, 1);
    
    return true;
}
