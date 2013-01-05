#include "CocosButton.h"

using namespace cocos2d;

CCocosButton *CCocosButton::s_pCurFocus = NULL;

//////////////////////////////////////////////////////////////////////////
//
// 虚函数
//
//////////////////////////////////////////////////////////////////////////

void CCocosButton::onReceiveNewTouch()
{
	// 获取到一个新的触摸信息
    
    if(NULL != s_pCurFocus && s_pCurFocus != this)
    {
        s_pCurFocus->loseFocus();
    }
    
	if(TYPE_SCALE == m_type)
	{
		m_pSprNormal->setScale(m_scaleFactor);
	}
    else if(TYPE_OPACITY == m_type)
    {
        m_pSprNormal->setOpacity(m_opaticyFactor);
    }
	else if(TYPE_REPLACE == m_type)
	{
		m_pSprNormal->setVisible(false);
		m_pSprReplace->setVisible(true);
	}
	else if(TYPE_TRANSPARENT == m_type)
	{
		m_pSprNormal->setVisible(true);
	}
	else if(TYPE_CHANGE == m_type)
	{
		if(m_pSprNormal->isVisible())
		{
			m_pSprNormal->setVisible(false);
			m_pSprReplace->setVisible(true);
		}
		else
		{
			m_pSprNormal->setVisible(true);
			m_pSprReplace->setVisible(false);
		}
	}
    
    
    m_bReceiveDown = true;
    
    s_pCurFocus = this;
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::onCurTouchMoveOut()
{
	// 获取到当前触摸移除BUTTON区域
    
	loseFocus();
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::onCurTouchFinish()
{
	// 获取当前触摸取消
    
	if(NULL != m_pListener)
	{
		m_pListener->onReceiveMsg(m_id, EVENT_UP);
	}
    
    loseFocus();
    
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::onExit()
{
    if(s_pCurFocus == this)
    {
        s_pCurFocus = NULL;
    }
    
    CCocosUIBase::onExit();
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::loseFocus()
{
    if(s_pCurFocus == this)
    {
        s_pCurFocus = NULL;
    }
    m_bReceiveDown = false;
    CCocosUIBase::loseFocus();
    ResetState();
}
//////////////////////////////////////////////////////////////////////////
//
// 私有函数
//
//////////////////////////////////////////////////////////////////////////


void CCocosButton::InitState(int id, CUIListener *pListerner, ButtonType type /* = TYPE_SCALE */, const char *szTexture1, const char *szTexture2, bool bUseFrame)
{
	CCocosUIBase::SetData(id, pListerner, true);
	m_type = type;
    
	if(bUseFrame)
	{
		if(NULL != szTexture1)
		{
			m_pSprNormal = CCSprite::createWithSpriteFrameName(szTexture1);
			addChild(m_pSprNormal);
		}
        
		if(NULL != szTexture2)
		{
			m_pSprReplace = CCSprite::createWithSpriteFrameName(szTexture2);
			addChild(m_pSprReplace);
		}
	}
	else
	{
		if(NULL != szTexture1)
		{
			m_pSprNormal = CCSprite::create(szTexture1);
			addChild(m_pSprNormal);
            
		}
        
		if(NULL != szTexture2)
		{
			m_pSprReplace = CCSprite::create(szTexture2);
			addChild(m_pSprReplace);
		}
	}
    
	if(type == TYPE_TRANSPARENT || type == TYPE_CALLSHOW)
	{
		m_pSprNormal->setVisible(false);
	}
	else if(type == TYPE_CHANGE || type	 == TYPE_REPLACE || type == TYPE_CALLCHANGE)
	{
		if(NULL != m_pSprReplace)
		{
			m_pSprReplace->setVisible(false);
		}
	}
    
	if(NULL != m_pSprNormal)
	{
		setContentSize(m_pSprNormal->getContentSize());
	}
    else if(NULL != m_pSprReplace)
    {
        setContentSize(m_pSprReplace->getContentSize());
    }
    
    m_bReceiveDown = false;
    
    if(NULL != m_pSprReplace)
    {
        AddFadeNode(m_pSprReplace);
    }
    if(NULL != m_pSprNormal)
    {
        AddFadeNode(m_pSprNormal);
    }
}
//////////////////////////////////////////////////////////////////////////
//
// 静态函数
//
//////////////////////////////////////////////////////////////////////////

CCocosButton *CCocosButton::Create(int id, CUIListener *pListener, ButtonType type /* = TYPE_SCALE */, bool bUseFrame /* = false */, 
                                   const char *szTexture1 /* = NULL */, const char *szTexture2 /* = NULL */)
{	
	CCocosButton *pNewButton = CCocosButton::create();
	pNewButton->InitState(id, pListener, type, szTexture1, szTexture2, bUseFrame);
	return pNewButton;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::ResetState()
{
	if(TYPE_SCALE == m_type)
	{
		m_pSprNormal->setScale(m_scaleNormal);
	}
    else if(TYPE_OPACITY == m_type)
    {
        m_pSprNormal->setOpacity(255);
    }
    
	else if(TYPE_REPLACE == m_type)
	{
		m_pSprNormal->setVisible(true);
		m_pSprReplace->setVisible(false);
	}
	else if(TYPE_TRANSPARENT == m_type)
	{
		m_pSprNormal->setVisible(false);
	}
    
	m_pCurTouch = NULL;
}

//////////////////////////////////////////////////////////////////////////
//
// 公有函数
//
//////////////////////////////////////////////////////////////////////////

CCocosButton::CCocosButton()
: m_pSprReplace(NULL)
, m_pSprNormal(NULL)
, m_pFntNormal(NULL)
, m_pFntReplace(NULL)
, m_scaleFactor(1.1f)
, m_scaleNormal(1.0f)
, m_opaticyFactor(128)
{
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::ChangeButton(bool bUseReplace /* = false */)
{
	if(bUseReplace)
	{
		m_pSprNormal->setVisible(false);
		m_pSprReplace->setVisible(true);
		m_pSprReplace->setPosition(this->getPosition());
	}
	else
	{
		m_pSprNormal->setVisible(true);
		m_pSprReplace->setVisible(false);
		m_pSprReplace->setPosition(this->getPosition());
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::CreateLable(const char *szFontFile, cocos2d::CCTextAlignment alignment, float scale, const char *szNormal, const char *szReplace)
{
	if(NULL == m_pFntNormal && NULL != szNormal && NULL != m_pSprNormal)
	{
		m_pFntNormal = CCLabelBMFont::create(szNormal, szFontFile);
		m_pFntNormal->setAlignment(alignment);
		m_pFntNormal->setPosition(ccp(m_pSprNormal->getContentSize().width*0.5f, m_pSprNormal->getContentSize().height*0.5f));
		m_pSprNormal->addChild(m_pFntNormal);
		m_pFntNormal->setScale(scale);
        AddFadeNode(m_pFntNormal);
	}

	if(NULL == m_pFntReplace && NULL != szReplace && NULL != m_pSprNormal)
	{
		m_pFntReplace = CCLabelBMFont::create(szReplace, szFontFile);
		m_pFntReplace->setAlignment(alignment);
		m_pFntReplace->setPosition(ccp(m_pSprReplace->getContentSize().width*0.5f, m_pSprReplace->getContentSize().height*0.5f));
		m_pSprReplace->addChild(m_pFntReplace);
		m_pFntReplace->setScale(scale);
        AddFadeNode(m_pFntReplace);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::SetScaleFactor(float scale) 
{
    if(m_pSprNormal  != NULL)
    {
        if(m_pSprNormal->getScale() == m_scaleFactor)
        {
            m_pSprNormal->setScale(scale);
        }
    }
    m_scaleFactor = scale;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::SetOpacityFactor(float opacity)
{
    if(m_pSprNormal  != NULL)
    {
        if(m_pSprNormal->getOpacity() == m_opaticyFactor)
        {
            m_pSprNormal->setScale(opacity);
        }
    }
    m_opaticyFactor = opacity;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::SetScaleNormal(float scale) 
{
    if(m_pSprNormal != NULL)
    {
        if(m_pSprNormal->getScale() == m_scaleNormal)
        {
            m_pSprNormal->setScale(scale);
        }
    }
    m_scaleNormal = scale;
}

//------------------------------------------------------------------------------------------------------------------------------------/
bool CCocosButton::IsChange()
{
    if(m_type == TYPE_CALLSCALE && m_pSprNormal->getScale() == m_scaleFactor)
    {
        return true;
    }
	if(NULL != m_pSprReplace && m_pSprReplace->isVisible())
	{
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::ShowRelpace()
{
    if(m_type == TYPE_CALLSCALE)
    {
        m_pSprNormal->setScale(m_scaleFactor);
    }
    else
    {
        if(NULL != m_pSprReplace)
        {
            m_pSprReplace->setVisible(true);
        }
        
        if(NULL != m_pSprNormal)
        {
            m_pSprNormal->setVisible(false);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::ShowNormal()
{
    if(m_type == TYPE_CALLSCALE)
    {
        m_pSprNormal->setScale(m_scaleNormal);
    }
    else
    {
        if(NULL != m_pSprReplace)
        {
            m_pSprReplace->setVisible(false);
        }
        
        if(NULL != m_pSprNormal)
        {
            m_pSprNormal->setVisible(true);
        }
    }
	
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosButton::ChangeState()
{
    if(IsChange())
    {
        ShowNormal();
    }
    else
    {
        ShowRelpace();
    }
}