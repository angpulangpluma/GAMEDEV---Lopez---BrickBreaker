#include "MainMenu.h"
#include "Story.h"
#include "Help.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	auto borders = this->getBoundingBox();
	auto title = Label::createWithSystemFont("DeAkumatize", "Arial", 30);

	title->setPosition(Point(borders.getMidX(), borders.getMidY()+150));

	this->addChild(title);

	menu_play = Label::createWithSystemFont("Play", "Arial", 30);
	menu_help = Label::createWithSystemFont("Help", "Arial", 30);
	menu_exit = Label::createWithSystemFont("Exit", "Arial", 30);

	menu_play->setPosition(Point(title->getPositionX(), title->getPositionY()-100));
	menu_help->setPosition(Point(menu_play->getPositionX(), menu_play->getPositionY() - 50));
	menu_exit->setPosition(Point(menu_help->getPositionX(), menu_help->getPositionY() - 50));

	this->addChild(menu_play);
	this->addChild(menu_help);
	this->addChild(menu_exit);

	//create "selector"
	selector = Sprite::create("ball.png");
	selector->setPosition(menu_play->getPositionX()-50, menu_play->getPositionY());
	this->addChild(selector);

	selection = 1;

	//keyboard listener
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
		Event* event){

		if (keys.find(keyCode) == keys.end()){
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}
		Vec2 pos = selector->getPosition();

		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_UP_ARROW:{
			log("up");
			if (selection == 1)
				selection = 3;
			else selection--;
		}; break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:{
			log("down");
			if (selection == 3)
				selection = 1;
			else selection++;
		}; break;

		}
		
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, selector);
	this->scheduleUpdate();
    return true;
}
void MainMenu::update(float delta){
	switch (selection){
	case 1: {
		selector->setPosition(menu_play->getPositionX() - 50, menu_play->getPositionY());
	}; break;
	case 2: {
		selector->setPosition(menu_help->getPositionX() - 50, menu_help->getPositionY());
	}; break;
	case 3: {
		selector->setPosition(menu_exit->getPositionX() - 50, menu_exit->getPositionY());
	}; break;
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_ENTER)){
		switch (selection){
		case 1:{
			play();
		}; break;
		case 2:{
			instructions();
		}; break;
		case 3:{
			exit();
		}; break;
		}
	}
}

bool MainMenu::isKeyPressed(cocos2d::EventKeyboard::KeyCode code){
	if (keys.find(code) != keys.end()){
		keys.erase(code);
		return true;
	}
	return false;
}
void MainMenu::play(){
	auto scene = Help::createScene();
	Director::getInstance()->pushScene(scene);
}

void MainMenu::instructions(){
	auto scene = Help::createScene();
	Director::getInstance()->pushScene(scene);
}

void MainMenu::exit(){
	Director::getInstance()->end();
}

int selection;
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> MainMenu::keys;
Sprite* selector;
