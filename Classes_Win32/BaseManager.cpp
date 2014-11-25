#include "BaseManager.h"
#include "Size.h"

USING_NS_CC;

bool BaseManager::init(){
	if(!Node::init()){
		return false;
	}


	createCoin();

	createRock();

	this->scheduleUpdate();

	return true;
}

void BaseManager::createCoin(){
	m_coinArr = Array::create();
	m_coinArr->retain();

	Coin* coin = NULL;

	for(int i = 1; i <= coinNum; ++ i){
		coin = Coin::create();

		coin->setVisible(false);

		coin->setTag(coinTag);

		//�ǵ����
		this->addChild(coin);

		m_coinArr->addObject(coin);
	}/**/
}

void BaseManager::createRock(){
	m_rockArr = Array::create();
	m_rockArr->retain();

	Rock* rock = NULL;

	float dis = 960;//����ʯ�ļ��

	for(int i = 1; i <= rockNum; ++ i){
		rock = Rock::create();

		rock->setTag(rockTag);

		rock->setPosition(dis,ground_hight+rock->getConSize().height/2);
		dis += 640;

		//�ǵ����
		this->addChild(rock);

		m_rockArr->addObject(rock);
	}/**/
}

void BaseManager::update(float dt){
	//auto visibleSize = Director::getInstance()->getVisibleSize();

	Ref* obj = NULL;
	Coin* coin = NULL;

	//��Ҫ���ý�ҵĸ���
	int setNum = 0;
	CCARRAY_FOREACH(m_coinArr,obj){
		coin = (Coin*)obj;

		//������Ļ
		if(coin->getPositionX() < -coin->getConSize().width/2){
			coin->setVisible(false);
		}

		//Ҫ�ǲ��ɼ���Ҫ�����ǲ�����Ļ�Ҫ�����Ǳ���ײ��
		if( !coin->isVisible() ){
			setNum ++;
		}

		//�ý���ƶ�
		coin->setPositionX(coin->getPositionX()-map_speed);
	}

	//4��ȫ������
	if(setNum == coinNum){
		int i = 0;
		float posX = 640+50;
		float posY = ground_hight + 15 +CCRANDOM_0_1()*60;
		CCARRAY_FOREACH(m_coinArr,obj){
			coin = (Coin*)obj;
			//������Ļ
			if(coin->getPositionX() < -coin->getConSize().width/2){
				coin->setVisible(false);
			}
			//Ҫ�ǲ��ɼ���Ҫ�����ǲ�����Ļ�Ҫ�����Ǳ���ײ��
			if( !coin->isVisible() ){
				//��������һ��
				if(i < 2){
					posX += 30;//�������ͬһ�߶ȣ����30
				}else{
					//��������λ�� 
					i = -1;
					posY = ground_hight + 15 +CCRANDOM_0_1()*60;
					posX += 400;
				}
				i ++;//���ý�Ҹ�����¼
				//
				coin->setVisible(true);
				coin->setPosition(posX,posY);
			}
		}
	}

	Ref* rockObj = NULL;
	Rock* rock = NULL;

	CCARRAY_FOREACH(m_rockArr,rockObj){
		rock = (Rock*)rockObj;

		if(rock->getPositionX() < -rock->getConSize().width/2){
			rock->setVisible(false);
		}
		if(rock->isVisible() == false){
			float posX = 1280;
			float posY = ground_hight + rock->getConSize().height/2;
			if(CCRANDOM_MINUS1_1() > 0){
				posY += 50;
			}
			rock->setPosition(posX,posY);
			rock->setVisible(true);
		}

		rock->setPositionX(rock->getPositionX()-map_speed);
	}
}
