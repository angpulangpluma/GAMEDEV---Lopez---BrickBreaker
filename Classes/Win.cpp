#include "MainMenu.h"
#include "Win.h"

USING_NS_CC;

Scene* WinScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = WinScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool WinScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto borders = this->getBoundingBox();
	auto title = Label::createWithSystemFont("You Won!", "Arial", 30);

	title->setPosition(Point(borders.getMidX(), borders.getMidY() + 150));

	this->addChild(title);

	menu_play = Label::createWithSystemFont("Play", "Arial", 30);
	menu_help = Label::createWithSystemFont("Help", "Arial", 30);
	menu_exit = Label::createWithSystemFont("Exit", "Arial", 30);

	menu_play->setPosition(Point(title->getPositionX(), title->getPositionY() - 100));
	menu_help->setPosition(Point(menu_play->getPositionX(), menu_play->getPositionY() - 50));
	menu_exit->setPosition(Point(menu_help->getPositionX(), menu_help->getPositionY() - 50));

	this->addChild(menu_play);
	this->addChild(menu_help);
	this->addChild(menu_exit);

	//create "selector"
	winselector = Sprite::create("ball.png");
	winselector->setPosition(menu_play->getPositionX() - 50, menu_play->getPositionY());
	this->addChild(winselector);

	winselect = 1;

	//keyboard listener
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
		Event* event){

		if (keys.find(keyCode) == keys.end()){
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}
		Vec2 pos = winselector->getPosition();

		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_UP_ARROW:{
			log("up");
			if (winselect == 1)
				winselect = 3;
			else winselect--;
		}; break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:{
			log("down");
			if (winselect == 3)
				winselect = 1;
			else winselect++;
		}; break;

		}

	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, winselector);
	this->scheduleUpdate();
	return true;
}
void WinScene::update(float delta){
	switch (winselect){
	case 1: {
		winselector->setPosition(menu_play->getPositionX() - 50, menu_play->getPositionY());
	}; break;
	case 2: {
		winselector->setPosition(menu_help->getPositionX() - 50, menu_help->getPositionY());
	}; break;
	case 3: {
		winselector->setPosition(menu_exit->getPositionX() - 50, menu_exit->getPositionY());
	}; break;
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_ENTER)){
		switch (winselect){
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

bool WinScene::isKeyPressed(cocos2d::EventKeyboard::KeyCode code){
	if (keys.find(code) != keys.end()){
		keys.erase(code);
		return true;
	}
	return false;
}
void WinScene::play(){
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}

void WinScene::instructions(){

}

void WinScene::exit(){
	Director::getInstance()->end();
}

int winselect;
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> WinScene::keys;
Sprite* winselector;
