#include "PlayScene.h"
#include "SimpleAudioEngine.h"
#include "GameOver.h"
#include "Size.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* PlayScene::createScene(){

	//�������������Scene
	auto scene = Scene::createWithPhysics();

	//��������,������������������ʹ����ɼ�
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	//Layer  ע������Ҫ�� auto ����ȻsetPhysicsWorld��������
	auto layer = PlayScene::create();

	layer->setPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);
	

	return scene;
}

bool PlayScene::init(){
	if(!Layer::init()){
		return false;
	}

	if(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()==false){
		SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3",true);
	}
	//���ﴴ���������
	initPhysicWorld();
	//ע��˳��,Ҫ�ڽ�ɫ֮ǰ.
	initBG(); 

	//������ɫ
	m_runner = Runner::create();
	m_runner->setPosition(runner_posX,ground_hight+m_runner->getRunJumpSize().height/2);
	m_runner->Run();
	this->addChild(m_runner);

	//����update  
    this->scheduleUpdate();	 

	createJumpButton();
	createCrouchButton();

	m_manager=BaseManager::create();
	this->addChild(m_manager);	

	return true;
}

void PlayScene::initPhysicWorld(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//����һ������߽� �ĸ���
	auto boundBody = PhysicsBody::createEdgeSegment(origin,
		ccp(visibleSize.width,0),
		PHYSICSBODY_MATERIAL_DEFAULT,1);

	//��һ��Node �����������
	auto boundNode = Node::create();

	boundNode->setPhysicsBody(boundBody);

	boundNode->setPosition(0,ground_hight);

	this->addChild(boundNode);
}

void PlayScene::initBG(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//����1
	bgSprite1 = Sprite::create("Map00.png");
	bgSprite1->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(bgSprite1);

	//����1
	groundSprite1 = Sprite::create("Ground00.png");
	groundSprite1->setPosition(visibleSize.width/2,groundSprite1->getContentSize().height/2);
	this->addChild(groundSprite1);

	//����2
	bgSprite2 = Sprite::create("Map01.png");
	bgSprite2->setPosition(bgSprite1->getContentSize().width+visibleSize.width/2,visibleSize.height/2);
	this->addChild(bgSprite2);
	
	//����2
	groundSprite2 = Sprite::create("Ground01.png");
	groundSprite2->setPosition(bgSprite1->getContentSize().width+visibleSize.width/2,groundSprite2->getContentSize().height/2);
	this->addChild(groundSprite2);
}

void PlayScene::update(float dt){
	int posX1 = bgSprite1->getPositionX();
	int posX2 = bgSprite2->getPositionX();

	posX1 -= map_speed;
	posX2 -= map_speed;

	auto mapSize = bgSprite1->getContentSize();

	if(posX1 < -mapSize.width/2){
		posX1 = mapSize.width + mapSize.width/2;
		posX2 = mapSize.width/2;
	}
	if(posX2 < -mapSize.width/2){
		posX2 = mapSize.width + mapSize.width/2;
		posX1 = mapSize.width/2;
	}

	bgSprite1->setPositionX(posX1);
	bgSprite2->setPositionX(posX2);
	groundSprite1->setPositionX(posX1);
	groundSprite2->setPositionX(posX2);
}

void PlayScene::createJumpButton(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//������Ļ��������
	auto jumpLabel = Label::create("Jump","Arail",30);
	auto norBtn = Scale9Sprite::create("norBtn.png");
	auto lightBtn = Scale9Sprite::create("lightBtn.png");

	auto jumpBtn = ControlButton::create(jumpLabel,norBtn);
	//������Ļ��������
	jumpBtn->setPosition(visibleSize.width-80,130);
	jumpBtn->setBackgroundSpriteForState(lightBtn,Control::State::HIGH_LIGHTED);

	jumpBtn->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(PlayScene::jumpEvent),
		Control::EventType::TOUCH_DOWN);

	this->addChild(jumpBtn);
}
void PlayScene::jumpEvent(Ref* pSender,Control::EventType event){
	SimpleAudioEngine::sharedEngine()->playEffect("jump.mp3");
	m_runner->Jump();
}

void PlayScene::createCrouchButton(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//������Ļ��������
	auto crouchLabel = Label::create("Crouch","Arail",30);
	auto norBtn = Scale9Sprite::create("norBtn.png");
	auto lightBtn = Scale9Sprite::create("lightBtn.png");

	auto crouchBtn = ControlButton::create(crouchLabel,norBtn);
	//������Ļ��������
	crouchBtn->setPosition(visibleSize.width-100,80);
	crouchBtn->setBackgroundSpriteForState(lightBtn,Control::State::HIGH_LIGHTED);

	crouchBtn->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(PlayScene::crouchDown),
		Control::EventType::TOUCH_DOWN);
	
	crouchBtn->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(PlayScene::crouchUp),
		Control::EventType::TOUCH_UP_INSIDE);

	this->addChild(crouchBtn);	
}
void PlayScene::crouchDown(Ref* pSender,Control::EventType event){
	//����ж�
	if(m_runner->getState() == running){
		m_runner->Crouch();
		//m_runner->setPosition(runner_posX,ground_hight+m_runner->getCrouchSize().height/2);
	}
	SimpleAudioEngine::sharedEngine()->playEffect("crouch.mp3");
}
void PlayScene::crouchUp(Ref* pSender,Control::EventType event){
	//��������������ʱ�򣬵��crouch �ɿ�֮��״̬�ͻ���running
	if(m_runner->getState() == crouch){
		m_runner->stopAllActions();
		m_runner->Run();
	    //m_runner->setPosition(runner_posX,ground_hight+m_runner->getRunJumpSize().height/2);
	}
}

void PlayScene::onEnter(){
	Layer::onEnter();
	auto contactListenner = EventListenerPhysicsContact::create();

	//CC_CALLBACK��ʾ�ص������Ĳ�������
	contactListenner->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin,this);

	//dispatcher = Director::getInstance()->getEventDispatcher();
	//_eventDispatcherӦ����һ��ʱ���ɷ���ȫ�ֱ�������ȫ������������ǵ�
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenner,this);
}
void PlayScene::onExit(){
	Layer::onExit();
	//ȡ���¼��ɷ�����
	//dispatcher->removeAllEventListeners();
	_eventDispatcher->removeEventListenersForTarget(this);
}
bool PlayScene::onContactBegin(PhysicsContact& contact){
	auto body_1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto body_2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
	
	CCLOG("ssssss");

	//�����ʾ��Ҫ�������ʯ��ײ
	if((body_1->getTag() == 3 && body_2->getTag() == 1) ||
		(body_1->getTag() == 1 && body_2->getTag() == 3) ){
			GameOver();
	}

	if(body_1->getTag() == 2){
		body_1->setVisible(false);
	}
	if(body_2->getTag() == 2){
		body_2->setVisible(false);
	}

	return false;
}
void PlayScene::GameOver(){
	SimpleAudioEngine::end();
	Director::getInstance()->replaceScene(GameOver::scene());
}


