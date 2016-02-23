#include "breakoutblocks.h"

USING_NS_CC;

Scene* Breakout::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
	auto *layer = Breakout::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
void Breakout::onEnter()
{
	Layer::onEnter();

	auto scene = getScene();
	auto world = scene->getPhysicsWorld();
	world->setGravity(Vec2::ZERO);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Breakout::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(Breakout::onContactSeperate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);

	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);

	this->addChild(edgeNode);
    
	walls = Vector<Sprite*>{};
	start = false;

	Sprite* one = Sprite::create("wall.png");
	one->setAnchorPoint(Vec2(0, 0));
	one->setPosition(0, 250);
	auto pb = PhysicsBody::createBox(Size(one->getBoundingBox().size.width, one->getBoundingBox().size.height), PhysicsMaterial(0.1f, 1.0f, 1.0f));
	pb->setDynamic(false);
	one->setPhysicsBody(pb);
	this->addChild(one, 0);
	walls.pushBack(one);
	auto posx = one->getPositionX();
	for (int i = 1; i < 1080/40; i++){
		Sprite* w = Sprite::create("wall.png");
		w->setAnchorPoint(Vec2(0, 0));
		w->setPosition(posx + 40, one->getPositionY());
		auto pb = PhysicsBody::createBox(Size(w->getBoundingBox().size.width, w->getBoundingBox().size.height), PhysicsMaterial(0.1f, 1.0f, 1.0f));
		pb->setDynamic(false);
		w->setPhysicsBody(pb);
		this->addChild(w, 0);
		posx = w->getPositionX();
		walls.pushBack(w);
	}

	platform = Sprite::create("platform.png");
	ball = Sprite::create("ball.png");
	ball->setAnchorPoint(Vec2(0, 0));
	platform->setPosition(this->getBoundingBox().getMidX()-30, this->getBoundingBox().getMidY() - 150);
	ball->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY() - 125);
	this->addChild(ball);
	platform->setAnchorPoint(Vec2(0, 0));

	auto physicsBody = PhysicsBody::createBox(Size(platform->getBoundingBox().size.width, platform->getBoundingBox().size.height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setContactTestBitmask(true);
	platform->setPhysicsBody(physicsBody);

	auto ballpb = PhysicsBody::createBox(Size(ball->getBoundingBox().size.width, ball->getBoundingBox().size.height), PhysicsMaterial(1.0f, 1.0f, 1.0f));
	//ballpb->setContactTestBitmask(true);
	ballpb->setVelocity(Vec2(0, 0));
	ball->setPhysicsBody(ballpb);

	this->addChild(platform, 0);

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

	this->scheduleUpdate();
    return;
}

void Breakout::update(float delta){

	Vec2 ploc = platform->getPosition();
	Vec2 bloc = ball->getPosition();
	auto pbody = ball->getPhysicsBody();
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE) && !start){
		start = true;
		log("start!");
		//make the ball move here
		pbody->setGravityEnable(false);
		pbody->setVelocity(Vec2(100, 100));
	}
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
		isKeyPressed(EventKeyboard::KeyCode::KEY_A)){
		log("left!");
		if (!start)
			ball->setPosition(ploc.x+30, bloc.y);
		
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

	if (bloc.y==0){
		//detect if the ball is at the bottom of the screen
	}
}

bool Breakout::isKeyPressed(EventKeyboard::KeyCode code){
	if (keys.find(code) != keys.end()){
		return true;
	}
	return false;
}

bool Breakout::onContactBegin(cocos2d::PhysicsContact& contact){
	auto a = contact.getShapeA()->getBody();
	auto b = contact.getShapeB()->getBody();

	float* v = new float[2];
	v[0] = a->getVelocity().length();
	v[1] = b->getVelocity().length();

	contact.setData(v);

	return true;
}

void Breakout::onContactSeperate(PhysicsContact& contact){
	auto a = contact.getShapeA()->getBody();
	auto b = contact.getShapeB()->getBody();

	float* v = (float*)contact.getData();
	auto va = a->getVelocity();
	auto vb = b->getVelocity();
	va.normalize();
	vb.normalize();
	a->setVelocity(va*v[0]);
	b->setVelocity(vb*v[1]);

	delete v;

}

std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> Breakout::keys;
