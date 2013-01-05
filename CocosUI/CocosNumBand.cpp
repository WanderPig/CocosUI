//
//  CocosNumBand.cpp
//  Archer2
//
//  Created by Wu Wang on 12-9-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CocosNumBand.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;

CCocosNumBand *CCocosNumBand::Create(bool bUseFrame, const char *szFormat, float space)
{
    CCocosNumBand *pNewBand = CCocosNumBand::create();
    pNewBand->setAnchorPoint(ccp(0, 0));
    pNewBand->ignoreAnchorPointForPosition(false);
    pNewBand->InitStateImage(bUseFrame, szFormat, space);
    return pNewBand;
}

//--------------------------------------------------------------------------------------------------------------------------/
CCocosNumBand *CCocosNumBand::Create(const char *szFontName, float size, cocos2d::ccColor3B color)
{
    CCocosNumBand *pNewBand = CCocosNumBand::create();
    pNewBand->InitStateFont(szFontName, size, color);
    return pNewBand;
}

//-------------------------------------------------------------------------------------------------------------------------/
CCocosNumBand::CCocosNumBand()
    : m_lastBitCount(0)
    , m_curBitCount(0)
    , m_space(0)
    , m_pLabelTTF(NULL)
    , m_countTimer(0)
    , m_pActOver(NULL)
    , m_idEffect(-1)
{
    memset(m_lastBits, 0, sizeof(char)*NUMCOUNT_MAX);
    memset(m_curBits, 0, sizeof(char)*NUMCOUNT_MAX);
    memset(m_pSprCur, NULL, sizeof(CCSprite *)*NUMCOUNT_MAX);
    m_strEffect = "";
}

//------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::InitStateImage(bool bUseFrame, const char *szFormat, float space)
{
    m_strFormat = szFormat;
    m_bUseFrame = bUseFrame;
    m_space     = space;
    m_bandType  = TYPE_IMAGE;
}

//--------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::InitStateFont(const char *szFontName, float size, cocos2d::ccColor3B color)
{
    m_pLabelTTF = CCLabelTTF::create("", szFontName, size);
    m_pLabelTTF->setColor(color);
    m_pLabelTTF->setAnchorPoint(ccp(0, 0.5));
    addChild(m_pLabelTTF);
    m_bandType = TYPE_FONT;
}

//--------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::ShowNum(long showNum)
{
    m_bandType == TYPE_IMAGE ? ShowNumImage(showNum) : ShowNumFont(showNum);
}

//--------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::ShowNumFont(long showNum)
{
    if(NULL == m_pLabelTTF)
    {
        return;
    }
    
    char szNumShow[32];
    sprintf(szNumShow, "%ld", showNum);
    m_pLabelTTF->setOpacity(255);
    m_pLabelTTF->setString(szNumShow);
}

//--------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::ShowNumImage(long showNum)
{
    CalNextBits(showNum);
    
    int curBit = 0;
    char tmpName[128];
    
    for(;curBit<m_curBitCount; curBit++)
    {
        if(m_pSprCur[curBit] != NULL)
        {
            m_pSprCur[curBit]->removeFromParentAndCleanup(true);
        }
    
        sprintf(tmpName, m_strFormat.c_str(), m_curBits[curBit]);
        if(m_bUseFrame)
        {
            m_pSprCur[curBit] = CCSprite::createWithSpriteFrameName(tmpName);
            
        }
        else
        {
            m_pSprCur[curBit] = CCSprite::create(tmpName);
        }
        
        addChild(m_pSprCur[curBit]);
    }
    
    for(;curBit<NUMCOUNT_MAX; curBit++)
    {
        if (NULL != m_pSprCur[curBit]) 
        {
            m_pSprCur[curBit]->removeFromParentAndCleanup(true);
            m_pSprCur[curBit] = NULL;
        }
    }
    
    m_lastBitCount = m_curBitCount;
    
    ResortNums();
}

//--------------------------------------------------------------------------------------------------------------------------/

void CCocosNumBand::RollNum(long showNum, bool bScale)
{
    CalNextBits(showNum);
    
    int curBit = 0;
    char tmpName[128];
    float curPos = 0;
    
    for(;curBit<m_curBitCount; curBit++)
    {
        if(m_curBits[curBit] != m_lastBits[curBit] || m_pSprCur[curBit] == NULL)
        {
            CCSprite *pSprNew = NULL;
            
            sprintf(tmpName, m_strFormat.c_str(), m_curBits[curBit]);
            if(m_bUseFrame)
            {
                pSprNew = CCSprite::createWithSpriteFrameName(tmpName);
            }
            else
            {
                pSprNew = CCSprite::create(tmpName);
            }
            addChild(pSprNew);
            
            curPos += pSprNew->getContentSize().width * 0.5f;
            pSprNew->setPosition(ccp(curPos, -pSprNew->getContentSize().height * 0.3f));
            
            pSprNew->runAction(CCMoveTo::create(0.15, ccp(curPos, 0)));
            if(NULL != m_pSprCur[curBit])
            {
                m_pSprCur[curBit]->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.15f,ccp(curPos, m_pSprCur[curBit]->getContentSize().height * 0.3f)), CCCallFuncN::create(this, callfuncN_selector(CCocosNumBand::OnRemoveNum))));
            }
            m_pSprCur[curBit] = pSprNew;
            
            curPos += pSprNew->getContentSize().width * 0.5f + m_space;
        }
        else
        {
            curPos += m_pSprCur[curBit]->getContentSize().width * 0.5f;
            m_pSprCur[curBit]->setPosition(ccp(curPos, 0));
            curPos += m_pSprCur[curBit]->getContentSize().width * 0.5f + m_space;
        }
    }
    
    for(;curBit<NUMCOUNT_MAX; curBit++)
    {
        if (NULL != m_pSprCur[curBit])
        {
            m_pSprCur[curBit]->removeFromParentAndCleanup(true);
            m_pSprCur[curBit] = NULL;
        }
    }
    
    m_lastBitCount = m_curBitCount;
    memcpy(m_lastBits, m_curBits, sizeof(m_curBits));
    
    if(bScale)
    {
        runAction(CCSequence::create(CCScaleTo::create(0.05f, 1.5f, 1.5f),
                                     CCScaleTo::create(0.05f, 1.0f, 1.0f),
                                     NULL));
    }
    
}

//-------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::OnRemoveNum(cocos2d::CCNode *pNode)
{
    pNode->removeFromParentAndCleanup(true);
}
//---------------------------------------------------------------------------------------------------------------------------/
//
// 计算下面要显示的值的各个位具体数字
//
//---------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::CalNextBits(long nextShowNum)
{
    int num[NUMCOUNT_MAX];
	int q = 1;
	for(int i=0; i<NUMCOUNT_MAX; i++)
	{
        
		num[i] = (nextShowNum % (10*q)) / q;
		q *= 10;
	}
    
	int bitNum = 0;
	for(; bitNum < NUMCOUNT_MAX; bitNum++)
	{
		if(num[NUMCOUNT_MAX -bitNum-1] != 0)
			break;
	}
    
	if(nextShowNum == 0)
	{
		bitNum = 1;
	}
	else
	{
		bitNum = NUMCOUNT_MAX-bitNum;
	}
    
	m_curBitCount = bitNum;
    
    for(int i=0; i<bitNum; i++)
    {
        m_curBits[i] = num[bitNum - i - 1];
    }
}

//----------------------------------------------------------------------------------------------------------------------------/
//
// 根据当前各位数字重新排列
//
//---------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::ResortNums()
{
    float curPos = 0;
    for(int i=0; i<m_lastBitCount; i++)
    {
        curPos += m_pSprCur[i]->getContentSize().width * 0.5f;
        m_pSprCur[i]->setPosition(ccp(curPos, 0));
        curPos += m_pSprCur[i]->getContentSize().width * 0.5f + m_space;
    }
}

//--------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::CountNum(long toNum,  cocos2d::CCAction *pAction, CountStyle style, long step, long fromNum)
{
    m_countTo     = toNum;
    m_countStyle  = style;
    m_countStep   = step;
    m_curCounting = fromNum;
    m_pActOver    = pAction;
    
    m_idEffect = SimpleAudioEngine::sharedEngine()->playEffect(m_strEffect.c_str());
    schedule(schedule_selector(CCocosNumBand::OnCount));
}

//--------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::OnCount(float dt)
{
    m_countTimer += dt;
    
	if(m_countTimer < 0.03)
	{
		return;
	}
	else
	{
		m_countTimer = 0;
	}
    
    
	if(m_curCounting < m_countTo)
	{
        
		long leftNum = m_countTo  - m_curCounting;
		long addNum = 1;
		while(leftNum >= 10)
		{
			leftNum /= 10;
			addNum *= 10;
		}
        
		m_curCounting += addNum;
        
		ShowNum(m_curCounting);
	}
	else
	{
        if(NULL != m_pActOver)
        {
            runAction(m_pActOver);
            m_pActOver = NULL;
        }
        
		
        ShowNum(m_countTo);
        
        if(m_idEffect != -1)
        {
            SimpleAudioEngine::sharedEngine()->stopEffect(m_idEffect);
        }
        m_idEffect = -1;
        unschedule(schedule_selector(CCocosNumBand::OnCount));
	}
}

//--------------------------------------------------------------------------------------------------------------------------/
void CCocosNumBand::StopCount()
{
    if(NULL != m_pActOver)
    {
        runAction(m_pActOver);
        m_pActOver = NULL;
    }
    unschedule(schedule_selector(CCocosNumBand::OnCount));
    ShowNum(m_countTo);
}

void CCocosNumBand::FadeOut(float d)
{
    if(m_bandType == TYPE_IMAGE)
    {
        for(int i=0; i<NUMCOUNT_MAX; i++)
        {
            m_pSprCur[i]->runAction(CCFadeOut::create(d));
        }
    }
    else
    {
        m_pLabelTTF->runAction(CCFadeOut::create(d));
    }
    
}



