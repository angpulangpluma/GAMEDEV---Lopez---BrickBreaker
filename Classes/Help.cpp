#include "Help.h"
#include "breakoutblocks.h"

USING_NS_CC;

Scene* Help::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Help::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Help::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto borders = this->getBoundingBox();
	auto title = Label::createWithSystemFont("How to Play", "Arial", 30);

	title->setPosition(Point(borders.getMidX(), borders.getMaxY() - 200));

	sequence = 1;

	this->addChild(title);

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
		Event* event){

		if (keys.find(keyCode) == keys.end()){
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}

	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, title);

	help_desc = Label::createWithSystemFont("Play", "Arial", 30);
	help_pic = Sprite::create("ball.png");
	help_pgno = Label::createWithSystemFont("1/4", "Arial", 25);
	auto help_play = Label::createWithSystemFont("Press 'Enter' to start playing!", "Arial", 25);

	help_desc->setPosition(Point(title->getPositionX(), title->getPositionY() - 50));
	help_pic->setPosition(Point(help_desc->getPositionX(), help_desc->getPositionY() - 200));
	help_pgno->setPosition(Point(borders.getMidX()+250, borders.getMinY()+20));
	help_play->setPosition(Point(borders.getMidX() - 250, borders.getMinY() + 20));

	this->addChild(help_desc);
	this->addChild(help_pic);
	this->addChild(help_pgno);
	this->addChild(help_play);

	this->scheduleUpdate();
    return true;
}

void Help::update(float delta){
	Node::update(delta);

	std::stringstream ss;
	switch (sequence){
	case 1:{
		ss << "1/4";
		help_pgno->setString(ss.str().c_str());
	}; break;
	case 2:{
		ss << "2/4";
		help_pgno->setString(ss.str().c_str());
	}; break;
	case 3:{
		ss << "3/4";
		help_pgno->setString(ss.str().c_str());
	}; break;
	case 4:{
		ss << "3/4";
		help_pgno->setString(ss.str().c_str());
	}; break;
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_ENTER)){
		auto scene = Breakout::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

bool Help::isKeyPressed(EventKeyboard::KeyCode code){
	if (keys.find(code) != keys.end()){
		return true;
	}
	return false;
}

int sequence;
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> Help::keys;
Label* help_desc;
Sprite* help_pic;
Label * help_pgno;
