#include "MainMenu.h"

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
    
    return true;
}

void MainMenu::play(Ref *pSender){

}

void MainMenu::instructions(Ref *pSender){

}

void MainMenu::exit(Ref *pSender){

}
