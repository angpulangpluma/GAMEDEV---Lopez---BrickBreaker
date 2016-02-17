#include "breakoutblocks.h"

USING_NS_CC;

Scene* Breakout::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = Breakout::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Breakout::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	walls = Vector<Sprite*>{};

	Sprite* one = Sprite::create("wall.png");
	one->setAnchorPoint(Vec2(0, 0));
	one->setPosition(0, 250);
	this->addChild(one, 1);
	walls.pushBack(one);
	auto posx = one->getPositionX();
	for (int i = 1; i < 1080/40; i++){
		Sprite* w = Sprite::create("wall.png");
		w->setAnchorPoint(Vec2(0, 0));
		w->setPosition(posx + 40, one->getPositionY());
		this->addChild(w, 1);
		posx = w->getPositionX();
		walls.pushBack(w);
	}

	platform = Sprite::create("platform.png");
	platform->setAnchorPoint(Vec2(0, 0));
	platform->setPosition(ball->getPosition());
	this->addChild(platform, 1);
	ball = Sprite::create("ball.png");
	ball->setAnchorPoint(Vec2(0, 0));
	ball->setPosition(platform->getPositionX(),platform->getPositionY()+50);
	this->addChild(ball, 1);

    return true;
}
