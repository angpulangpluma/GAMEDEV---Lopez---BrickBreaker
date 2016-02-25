#include "breakoutblocks.h"

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

	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, 
		visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	edgeNode->setTag(49);

	this->addChild(edgeNode);

	{
		start = false;

		auto one = Sprite::create("wall.png");
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
		auto posx = one->getPositionX();
		for (int i = 1; i < 1080 / 40; i++){
			Sprite* w = Sprite::create("wall.png");
			w->setAnchorPoint(Vec2(0, 0));
			w->setPosition(posx + 40, one->getPositionY());
			auto pb = PhysicsBody::createBox(w->getContentSize(), 
				PhysicsMaterial(2.0f, 1.0f, 2.0f));
			pb->setDynamic(false);
			pb->setCollisionBitmask(1);
			pb->setContactTestBitmask(true);
			pb->setGravityEnable(false);
			w->setPhysicsBody(pb);
			w->setTag(39);
			this->addChild(w);
			posx = w->getPositionX();
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
		platform->setPosition(this->getBoundingBox().getMidX() - 30, this->getBoundingBox().getMidY() - 150);
		ball->setPosition(platform->getPositionX() + 30, platform->getPositionY() + 25);

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

void Breakout::createObject(){
	log("new object pls");

	auto sp2 = Sprite::create("ball.png");
	sp2->setPosition(Vec2(random(3, 1080), random(3, 480)));
	while (sp2->getPosition() == sp1->getPosition()){
		sp2->setPosition(Vec2(random(0, 1080), random(0, 480)));
	}
	sp2->setTag(11);

	auto sp2bd = PhysicsBody::createBox(sp2->getContentSize(), 
		PhysicsMaterial(1.0f, 1.0, 1.0f));
	sp2bd->setCollisionBitmask(1);
	sp2bd->setContactTestBitmask(true);
	sp2bd->setGravityEnable(false);
	sp2bd->setVelocity(Vec2(random(-500, 500), random(-500, 500)));

	sp2->setPhysicsBody(sp2bd);

	this->addChild(sp2);
	log("created new object!");
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
		pbody->setVelocity(Vec2(100, 100));
		//pbody->setAngularVelocity(0);
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

	if (bloc.y<platform->getBoundingBox().getMinY()){
		//detect if the ball is at the bottom of the screen
		log("nooooo");
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
			//createObject();
		}
		else if (b->getTag() == 39 && a->getTag() == 11){
			log("got in");
			b->removeFromParentAndCleanup(true);
			//createObject();
		}
		else if (a->getTag() == 49 && b->getTag() == 11){
			float* v = new float[2];
			v[0] = b->getPhysicsBody()->getVelocity().length();
			v[1] = b->getPhysicsBody()->getVelocity().length();

			contact.setData(v);
		}
		else if (b->getTag() == 49 && a->getTag() == 11){
			float* v = new float[2];
			v[0] = a->getPhysicsBody()->getVelocity().length();
			v[1] = a->getPhysicsBody()->getVelocity().length();

			contact.setData(v);
		}
		else if (a->getTag() == b->getTag() && a->getTag() == 11){
			float* v = new float[2];
			v[0] = a->getPhysicsBody()->getVelocity().length();
			v[1] = b->getPhysicsBody()->getVelocity().length();

			contact.setData(v);
		}
		/*if (a->getTag() == 39){
			a->removeFromParentAndCleanup(true);
			createObject();
		}
		else if (b->getTag() == 39){
			b->removeFromParentAndCleanup(true);
			createObject();
		}
		else if (a->getTag() == 49){

		}
		else if (b->getTag() == 49){

		}*/
	}

	/*if (a && b){
		log("Something happened!");
		if (a->getTag() == 39 && 1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()){
			log("Collision!");
			a->setVelocity(Vec2(100, 100));
			//b->setAngularDamping(0.0f);
			for (int i = 0; i < walls.size(); i++){
				if (b->getNode()->getTag() == walls.at(i)->getTag()){
					walls.at(i)->removeFromParentAndCleanup(true);
				}
				else{
					log("No wall detected");
				}
			}
			//b->getNode()->removeFromParentAndCleanup(true);
		}
		else if (b->getTag() == 39 && 1 == b->getCollisionBitmask() && 2 == a->getCollisionBitmask()){
			log("Collision!");
			b->setVelocity(Vec2(100, 100));
			//b->setAngularDamping(0.0f);
			for (int i = 0; i < walls.size(); i++){
				if (a->getNode()->getTag() == walls.at(i)->getTag()){
					walls.at(i)->removeFromParentAndCleanup(true);
				}
				else{
					log("No wall detected");
				}
			}
		}
	}*/

	return true;
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
