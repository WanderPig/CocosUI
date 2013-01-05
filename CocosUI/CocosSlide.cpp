#include "CocosSlide.h"

using namespace cocos2d;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 虚函数
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCocosSlide::onReceiveNewTouch()
{
	m_power = convertTouchToNodeSpace(m_pCurTouch).x + m_barWidth*0.5f;
	m_lastTouchPosX = m_power;
	
	ChangeState();
    
    
	m_pListener->onReceiveMsg(m_id);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosSlide::onCurTouchMoving()
{
	m_power = convertTouchToNodeSpace(m_pCurTouch).x + m_barWidth*0.5f;
	float xAdd = m_power - m_lastTouchPosX;
	m_lastTouchPosX = m_power;
    
    
	m_power += xAdd;
    
    
	ChangeState();
    
	m_pListener->onReceiveMsg(m_id);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 静态成员函数
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CCocosSlide *CCocosSlide::Create(int id , CUIListener *pListener, bool bUseFrame, const char *szBarName, 
                                 const char *szBtnName /* = NULL  */, const char *szBackName /* = NULL */)
{
	CCocosSlide *pSlide = CCocosSlide::create();
	pSlide->InitState(id, pListener, bUseFrame, szBarName, szBtnName, szBackName);
	return pSlide;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 私有成员函数
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCocosSlide::InitState(int id, CUIListener *pListener, bool bUseFrame, 
							const char *szBarName, const char *szBtnName /* = NULL  */, const char *szBackName /* = NULL */)
{
	if(NULL == szBarName)
	{
		return false;
	}
    
	SetData(id, pListener);
    
	if(bUseFrame)
	{	
		if(NULL != szBackName)
		{
			m_pSprBack = CCSprite::createWithSpriteFrameName(szBackName);
			addChild(m_pSprBack);
		}
        
		m_pSprBar = CCSprite::createWithSpriteFrameName(szBarName);
		addChild(m_pSprBar);
        
		if(NULL != szBtnName)
		{
			m_pSprButton = CCSprite::createWithSpriteFrameName(szBtnName);
			addChild(m_pSprButton);
		}
	}
	else
	{
		if(NULL != szBackName)
		{
			m_pSprBack = CCSprite::create(szBackName);
			addChild(m_pSprBack);
		}
        
		m_pSprBar = CCSprite::create(szBarName);
		addChild(m_pSprBar);
		
        
		if(NULL != szBtnName)
		{
			m_pSprButton = CCSprite::create(szBtnName);
			addChild(m_pSprButton);
			
		}
	}
    
	m_pSprBar->setAnchorPoint(ccp(0, 0.5f));	
	m_pSprBar->setPosition(ccp(-m_pSprBar->getTextureRect().size.width*0.5f, 0));
    
	setContentSize(m_pSprBar->getTextureRect().size);
    
    m_barWidth = m_pSprBar->getTextureRect().size.width;
    
	ChangeState();
    
	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosSlide::ChangeState()
{
	if(m_power> m_barWidth)
	{
		m_power = m_barWidth;
	}
	else if(m_power <=0)
	{
		m_power = 0;
	}
    
	m_pSprBar->setTextureRect(CCRect(0, 0 , m_power, m_pSprBar->getTextureRect().size.height));
    
	if(NULL != m_pSprButton)
	{
		m_pSprButton->setPosition(ccp(m_power - m_barWidth *0.5f, 0));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 公有成员函数
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CCocosSlide::CCocosSlide(void)
: m_pSprBack(NULL)
, m_pSprBar(NULL)
, m_pSprButton(NULL)
, m_weight(1)
, m_power(0)
{
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosSlide::SetPower(float power)  
{
	assert(power >= 0);
    
	m_power = power *  m_barWidth / m_weight;
    
	ChangeState();
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosSlide::SetWeight(float weight) 
{
	assert(weight > 0);
    
	m_weight = weight;
}
float CCocosSlide::GetPower()
{
	return m_power / m_barWidth * m_weight;
}


