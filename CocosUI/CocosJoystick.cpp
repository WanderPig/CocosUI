#include "CocosJoystick.h"
#include <cmath>
using namespace cocos2d;

float CalDirection(float xStart,float yStart, float xEnd, float yEnd)
{
	double accDir=0;
	float vecX = xEnd - xStart;
	float vecY = yEnd - yStart;
    
	// 在X轴上，90°或270°
	if(abs(vecX) < 0.00001f)
	{
		if(vecY > 0)
		{
			accDir = 0;
		}
		else if(vecY < 0)
		{
			accDir = 3.141592;
		}
	}
	else
	{
		// 计算加速方向（弧度）
		accDir = atan(vecY / vecX);
        
		if(vecX >=0)
		{
			// 第一，四象限
			accDir = 3.141592*0.5 - accDir;
		}
		else
		{
			// 第二，三象限
			accDir = 3.141592*1.5 - accDir;
		}
	}
    
	return (float)(accDir * 180.0f / 3.141592);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosJoystick::onReceiveNewTouch()
{
	CCPoint ptCur = convertTouchToNodeSpace(m_pCurTouch);
	if(m_bCanMove)
	{
		m_ptCenter = ptCur;
		m_pSprBack->stopAllActions();
		m_pSprBack->setOpacity(255);
		m_pSprBack->setVisible(true);
		m_pSprBack->setPosition(m_ptCenter);
		m_pSprCenter->stopAllActions();
		m_pSprCenter->setOpacity(255);
		m_pSprCenter->setVisible(true);
		m_pSprCenter->setPosition(m_ptCenter);
		m_curRotation = -1;
		m_curPower    = 0;
	}
	else
	{
		m_pSprCenter->stopAllActions();
		onCurTouchMoving();
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosJoystick::onCurTouchMoving()
{
	
	CCPoint ptCur = convertTouchToNodeSpace(m_pCurTouch);
    
	float rotation = CalDirection(m_ptCenter.x, m_ptCenter.y, m_ptCur.x, m_ptCur.y);
	float dist     = ccpDistance(ptCur, m_ptCenter);
	float distFinal = dist;
    
	if(distFinal > m_maxRadius)
	{
		distFinal = m_maxRadius;
	}
    
	
	CCPoint ptInc = CCPoint(distFinal * (ptCur.x - m_ptCenter.x) / dist, distFinal * (ptCur.y - m_ptCenter.y) / dist);
    
	m_pSprCenter->setPosition(ccpAdd(m_ptCenter, ptInc));
    
	m_curRotation = rotation;
	m_curPower    = dist / m_maxRadius;
	m_curPowerX   = ptInc.x / m_maxRadius;
	m_curPowerY   = ptInc.y / m_maxRadius;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosJoystick::onCurTouchFinish()
{
	
	m_pSprCenter->runAction(CCMoveTo::create(m_resetDuration, m_ptCenter));
    
	if(m_bCanMove)
	{
		m_pSprCenter->runAction(CCFadeOut::create(m_resetDuration));
		m_pSprBack->runAction(CCFadeOut::create(m_resetDuration));
	}
	m_curRotation = -1;
	m_curPower    = 0;
	m_curPowerX   = 0;
	m_curPowerY   = 0;
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosJoystick *CCocosJoystick::CreateJoystick(const char *szJoyPath, const char *szJoybkPath, bool isCanMove)
{
    
	CCocosJoystick *pJoystick = CCocosJoystick::create();
    
	pJoystick->InitState(szJoyPath, szJoybkPath, isCanMove);
    
	return pJoystick;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosJoystick::InitState(const char *szJoyPath, const char *szJoybkPath, bool isCanMove)
{
	m_pSprBack = CCSprite::create(szJoybkPath);
	m_pSprCenter = CCSprite::create(szJoyPath);
    
	if(isCanMove)
	{
		m_pSprBack->setVisible(false);
		m_pSprCenter->setVisible(false);
		setPosition(ccp(getContentSize().width*0.5f, getContentSize().height*0.5f));
	}
	else
	{
		setContentSize(m_pSprBack->getContentSize());
	}
    
	addChild(m_pSprBack);
	addChild(m_pSprCenter);
    
	m_bCanMove = isCanMove;
    
	m_maxRadius = m_pSprBack->getContentSize().width*0.5f;
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosJoystick::CCocosJoystick()
: m_pSprBack(NULL)
, m_pSprCenter(NULL)
, m_bCanMove(false)
, m_curPower(0)
, m_curPowerX(0)
, m_curPowerY(0)
, m_maxRadius(0)
, m_resetDuration(1.0f)
, m_ptCenter(ccp(0, 0))
{
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosJoystick::SetMoveJoyArea(float x, float y, float width, float height)
{
	if(!m_bCanMove)
	{
		return;
	}
    
	setContentSize(CCSize(width, height));
	setPosition(ccp(x +width *0.5f, y + height*0.5f));
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosJoystick::ResetState()
{
	m_pCurTouch = NULL;
	m_curRotation = -1;
	m_curPower    = 0;
	m_curPowerX   = 0;
	m_curPowerY   = 0;
}