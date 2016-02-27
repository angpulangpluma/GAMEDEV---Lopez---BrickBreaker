#include "SelectCharacter.h"

USING_NS_CC;

Scene* SelectCharacter::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectCharacter::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectCharacter::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto borders = this->getBoundingBox();
	auto title = Label::createWithSystemFont("DeAkumatize", "Arial", 30);

	title->setPosition(Point(borders.getMidX(), borders.getMidY() + 150));

	this->addChild(title);

	auto menu_play = Label::createWithSystemFont("Play", "Arial", 30);
	auto menu_help = Label::createWithSystemFont("Help", "Arial", 30);
	auto menu_exit = Label::createWithSystemFont("Exit", "Arial", 30);

	menu_play->setPosition(Point(title->getPositionX(), title->getPositionY() - 100));
	menu_help->setPosition(Point(menu_play->getPositionX(), menu_play->getPositionY() - 50));
	menu_exit->setPosition(Point(menu_help->getPositionX(), menu_help->getPositionY() - 50));

	this->addChild(menu_play);
	this->addChild(menu_help);
	this->addChild(menu_exit);

    
    return true;
}

void SelectCharacter::update(float delta){

}
bool SelectCharacterisKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode){
	return true;
}

int characterselect;
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> keys;
cocos2d::Label * char_desc;
cocos2d::Sprite * char_pic;
