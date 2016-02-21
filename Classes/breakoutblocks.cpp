#include "breakoutblocks.h"

USING_NS_CC;

Scene* Breakout::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
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
	start = false;

	Sprite* one = Sprite::create("wall.png");
	one->setAnchorPoint(Vec2(0, 0));
	one->setPosition(0, 250);
	this->addChild(one, 0);
	walls.pushBack(one);
	auto posx = one->getPositionX();
	for (int i = 1; i < 1080/40; i++){
		Sprite* w = Sprite::create("wall.png");
		w->setAnchorPoint(Vec2(0, 0));
		w->setPosition(posx + 40, one->getPositionY());
		this->addChild(w, 0);
		posx = w->getPositionX();
		walls.pushBack(w);
	}

	platform = Sprite::create("platform.png");
	ball = Sprite::create("ball.png");
	ball->setAnchorPoint(Vec2(0, 0));
	platform->setPosition(this->getBoundingBox().getMidX()-30, this->getBoundingBox().getMidY() - 150);
	ball->setPosition(this->getBoundingBox().getMidX(), platform->getPositionY() + 25);
	platform->setAnchorPoint(Vec2(0, 0));
	this->addChild(platform, 0);
	this->addChild(ball, 0);

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
		Event* event){

		if (keys.find(keyCode) == keys.end()){
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}

		Vec2 loc = platform->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:{ //left
			platform->setPosition(loc.x-5, loc.y);
			log("left");
		};  break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:{ //right
			platform->setPosition(loc.x+5, loc.y);
			log("right");
		}; break;
		}
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, platform);

	this->scheduleUpdate();
    return true;
}

void Breakout::update(float delta){

	Vec2 ploc = platform->getPosition();
	Vec2 bloc = ball->getPosition();
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE) && !start){
		start = true;
		log("start!");
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
		isKeyPressed(EventKeyboard::KeyCode::KEY_A)){
		log("left!");
		if (!start)
			ball->setPosition(ploc.x+30, bloc.y);
		platform->setPosition(ploc.x-5, ploc.y);
	}
	else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
		isKeyPressed(EventKeyboard::KeyCode::KEY_D)){
		log("right!");
		if (!start)
			ball->setPosition(ploc.x+35, bloc.y);
		platform->setPosition(ploc.x+5, ploc.y);
	}
}

bool Breakout::isKeyPressed(EventKeyboard::KeyCode code){
	if (keys.find(code) != keys.end()){
		return true;
	}
	return false;
}

std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> Breakout::keys;
