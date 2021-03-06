#include "breakoutblocks.h"
#include "MainMenu.h"
#include "Win.h"
#include "Lose.h"

USING_NS_CC;

Scene* Breakout::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
	auto layer = Breakout::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Breakout::init()
{
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, 
		PhysicsMaterial(0.0f, 1.0f, 0.0f), 3);
	edgeBody->setGravityEnable(false);
	//auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	//edgeBody->setCollisionBitmask(1);
	//edgeBody->setContactTestBitmask(true);

	edgeBox = Node::create();
	edgeBox->setPosition(Point(visibleSize.width / 2 + origin.x, 
		visibleSize.height / 2 + origin.y));
	edgeBox->setPhysicsBody(edgeBody);
	edgeBox->setTag(49);

	this->addChild(edgeBox);

	{
		start = false;

		/*auto one = Sprite::create("wall.png");
		one->setAnchorPoint(Vec2(0, 0));
		one->setPosition(0, 250);
		auto onepb = PhysicsBody::createBox(one->getContentSize(),
		PhysicsMaterial(2.0f, 1.0f, 2.0f));
		onepb->setDynamic(false);
		onepb->setCollisionBitmask(1);
		onepb->setContactTestBitmask(true);
		onepb->setGravityEnable(false);
		one->setPhysicsBody(onepb);
		one->setTag(39);
		this->addChild(one);
		wallcount = 1;*/
		auto ycoord = 400;
		for (int i = 0; i < 5; i++){
			createWalls(0, ycoord);
			ycoord -= 45;
		}
		

		ball = Sprite::create("ball.png");
		auto ballpb = PhysicsBody::createBox(ball->getContentSize(), 
			PhysicsMaterial(1.0f, 1.0, 1.0f));
		ballpb->setVelocity(Vec2(0, 0));
		ballpb->setCollisionBitmask(1);
		ballpb->setContactTestBitmask(true);
		ballpb->setGravityEnable(false);
		ball->setTag(11);
		ball->setPhysicsBody(ballpb);

		platform = Sprite::create("platform.png");
		platform->setAnchorPoint(Vec2(0, 0));
		platform->setPosition(this->getBoundingBox().getMidX() - 30, this->getBoundingBox().getMinY() + 50);
		ball->setPosition(platform->getPositionX() + 35, platform->getPositionY() + 25);

		auto physicsBody = PhysicsBody::createBox(platform->getContentSize(),
			PhysicsMaterial(2.0f, 1.0f, 2.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCollisionBitmask(10);
		physicsBody->setContactTestBitmask(true);
		physicsBody->setGravityEnable(false);
		platform->setTag(10);
		platform->setPhysicsBody(physicsBody);

		this->addChild(ball);
		this->addChild(platform);

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

				platform->setPosition(loc.x - 5, loc.y);
				log("left");

			};  break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:{ //right

				platform->setPosition(loc.x + 5, loc.y);
				log("right");

			}; break;
			}
		};

		eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
			keys.erase(keyCode);
		};

		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, platform);

	}

	
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Breakout::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(Breakout::onContactSeperate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();
    return true;
}

void Breakout::createWalls(float startCoordX, float startCoordY){
	auto posx = startCoordX;
	auto prev = Sprite::create("wall.png");
	for (int i = 1; i < 1080 / 40; i++){
		Sprite* w = Sprite::create("wall.png");
		w->setAnchorPoint(Vec2(0, 0));
		w->setPosition(posx, startCoordY);
		auto pb = PhysicsBody::createBox(w->getContentSize(),
		PhysicsMaterial(2.0f, 1.0f, 2.0f));
		pb->setDynamic(false);
		pb->setCollisionBitmask(1);
		pb->setContactTestBitmask(true);
		pb->setGravityEnable(false);
		w->setPhysicsBody(pb);
		w->setTag(39);
		this->addChild(w);
		posx = w->getPositionX()+45;
		prev = w;
		wallcount++;
	}
}

void Breakout::update(float delta){

	Vec2 ploc = platform->getPosition();
	Vec2 bloc = ball->getPosition();
	auto pbody = ball->getPhysicsBody();
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE) && !start){
		start = true;
		log("start!");
		//make the ball move here
		pbody->setLinearDamping(0.0f);
		pbody->setVelocity(Vec2(250, 250));
		keys.erase(EventKeyboard::KeyCode::KEY_SPACE);
		//pbody->setAngularVelocity(0);
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
		isKeyPressed(EventKeyboard::KeyCode::KEY_A)){
		log("left!");
		if (!start)
			ball->setPosition(ploc.x + 30, bloc.y);
		platform->setPosition(ploc.x - 5, ploc.y);
		log("left");

	}
	else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
		isKeyPressed(EventKeyboard::KeyCode::KEY_D)){
		log("right!");
		if (!start)
			ball->setPosition(ploc.x+35, bloc.y);
		
			platform->setPosition(ploc.x + 5, ploc.y);
			log("left");
		
	}

	if (bloc.y<platform->getBoundingBox().getMaxY()){
		//detect if the ball is at the bottom of the screen
		auto scene = LoseScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
	else if (wallcount == 0){
		auto scene = WinScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

bool Breakout::isKeyPressed(EventKeyboard::KeyCode code){
	if (keys.find(code) != keys.end()){
		return true;
	}
	return false;
}

bool Breakout::onContactBegin(cocos2d::PhysicsContact& contact){
	log("Contact has been established!");
	auto a = contact.getShapeA()->getBody()->getNode();
	auto b = contact.getShapeB()->getBody()->getNode();

	if (a && b){
		if (a->getTag() == 39 && b->getTag() == 11){
			log("got in");
			a->removeFromParentAndCleanup(true);
			ball->getPhysicsBody()->setVelocity(Vec2(250, 250));
			return true;
			//createObject();
		}
		else if (b->getTag() == 39 && a->getTag() == 11){
			log("got in");
			b->removeFromParentAndCleanup(true);
			ball->getPhysicsBody()->setVelocity(Vec2(250, 250));
			return true;
			//createObject();
		}
		else if (a->getTag() == 49 && b->getTag() == 11){
			ball->getPhysicsBody()->setVelocity(Vec2(250, 250));
			return false;
		}
		else if (b->getTag() == 49 && a->getTag() == 11){
			ball->getPhysicsBody()->setVelocity(Vec2(250, 250));
			return false;
		}
	}
}

void Breakout::onContactSeperate(PhysicsContact& contact){
	auto a = contact.getShapeA()->getBody();
	auto an = contact.getShapeA()->getBody()->getNode();
	auto b = contact.getShapeB()->getBody();
	auto bn = contact.getShapeB()->getBody()->getNode();
	
	if (a->getTag() == b->getTag() && a->getTag() == 11){
		float* v = (float*)contact.getData();
		auto va = a->getVelocity();
		auto vb = b->getVelocity();
		va.normalize();
		vb.normalize();
		a->setVelocity(va*v[0]);
		b->setVelocity(vb*v[1]);

		delete v;
	}

}

std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> Breakout::keys;
Sprite* sp1;
