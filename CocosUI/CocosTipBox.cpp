#include "CocosTipBox.h"
#include "../Common/InlineFunc.h"

using namespace cocos2d;


CCocosTipBox *CCocosTipBox::s_pCurTipBox = NULL;

bool CCocosTipBox::ShowCurTip(const char *szMsg, float waitTimer, long style, int id, CUIListener *pListener, cocos2d::CCAction *pAction)
{
    if(NULL == s_pCurTipBox)
    {
        return false;
    }
    
    s_pCurTipBox->ShowTip(szMsg, waitTimer, style, id, pListener, pAction);
    
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::CancelCurTip()
{
    if(NULL == s_pCurTipBox)
    {
        return;
    }
    
    s_pCurTipBox->CancelWait();
}

//----------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::onEnter()
{
    if(s_pCurTipBox != this)
    {
        s_pCurTipBox = this;
    }
    CCLayer::onEnter();
}

//----------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::onExit()
{
    if(s_pCurTipBox == this)
    {
        s_pCurTipBox = NULL;
    }
    
    CCLayer::onExit();
}

//----------------------------------------------------------------------------------------------------------------------------/
CCocosTipBox::CCocosTipBox()
	: m_pSprBack(NULL)
    , m_pManager(NULL)
    , m_pBtnOk(NULL)
    , m_pBtnCancel(NULL)
{

}

//---------------------------------------------------------------------------------------------------------------------------/
CCocosTipBox *CCocosTipBox::Create(BoxType type, const char *szName, float sizeOrScale, unsigned char alpha, bool bUseFrame, const char *szBoxTexture)
{
	ccColor4B color = {0, 0, 0, alpha};
    CCSize sScreen = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCocosTipBox *pNewTipBox =  CCocosTipBox::create();
    CCLayerColor *pLayer = CCLayerColor::create(color, sScreen.width*2.0f, sScreen.height*2.0f);
    pLayer->setPosition(ccp(-pLayer->getContentSize().width*0.5f, -pLayer->getContentSize().height*0.5f));
    pNewTipBox->addChild(pLayer);
    
	pNewTipBox->InitState(type, szName, sizeOrScale, bUseFrame, szBoxTexture);

	return pNewTipBox;
}

//---------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::InitState(BoxType type, const char *szName, float sizeOrScale,bool bUseFrame, const char *szBoxTexture)
{
	if(type == TYPE_BMFONT)
	{
		m_pLabel = CCLabelBMFont::create("", szName);
		((CCLabelBMFont *)m_pLabel)->setScale(sizeOrScale);
		addChild(((CCLabelBMFont *)m_pLabel), LAYER_MSG);
		
	}
	else
	{
		m_pLabel = CCLabelTTF::create("", szName, sizeOrScale);
		addChild(((CCLabelTTF *)m_pLabel), LAYER_MSG);
	}

	if(szBoxTexture != NULL)
	{
		if(bUseFrame)
		{
			m_pSprBack = CCSprite::createWithSpriteFrameName(szBoxTexture);
		}
		else
		{
			m_pSprBack = CCSprite::create(szBoxTexture);
		}


		addChild(m_pSprBack, LAYER_GROUND);
		
	}

	setVisible(false);
}

//----------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::SetOkCancel(CCocosButton *pBtnOk, CCocosButton *pBtnCancel)
{
    
    if (NULL != m_pBtnOk)
    {
        m_pBtnOk->removeFromParentAndCleanup(true);
    }
    
    if(NULL != m_pBtnCancel)
    {
        m_pBtnCancel->removeFromParentAndCleanup(true);
    }
    
    m_pBtnOk = pBtnOk;
    m_pBtnCancel = pBtnCancel;
    
    if(NULL != m_pBtnOk)
    {
        m_ptOk = m_pBtnOk->getPosition();
        m_pBtnOk->SetData(CONTROL_OK, this, false);
        addChild(m_pBtnOk, LAYER_BUTTON);
    }
    
    if(NULL != m_pBtnCancel)
    {
        m_ptCancel = m_pBtnCancel->getPosition();
        m_pBtnCancel->SetData(CONTROL_OK, this, false);
        addChild(m_pBtnCancel, LAYER_BUTTON);
    }
    

}
//----------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::CancelWait()
{
    if(m_pManager != NULL)
    {
        m_pManager->EnableControls(true);
    }
	setVisible(false);
    m_pListener = NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::ShowTip(const char *szMsg, float waitTimer, long style, int id, CUIListener *pListener, cocos2d::CCAction *pAction)
{
    SetData(id, pListener);
	if(NULL != szMsg && style & STYLE_MSG)
	{
		m_pLabel->setString(szMsg);
	}
    else
    {
        m_pLabel->setString("");
    }
    
    if (NULL != m_pSprBack)
    {
        if(style & STYLE_BACKGROUND)
        {
            m_pSprBack->setVisible(true);
        }
        else
        {
            m_pSprBack->setVisible(false);
        }
    }
    
    if(style & STYLE_OK)
    {
        if(NULL != m_pBtnOk)
        {
            m_pBtnOk->show(true);
            m_pBtnOk->setPosition(m_ptOk);
        }
        
        if(!(style & STYLE_CANCEL))
        {
            m_pBtnOk->setPosition(ccp((m_ptOk.x + m_ptCancel.x)*0.5f, (m_ptOk.y + m_ptCancel.y)*0.5f));
        }
    }
    else
    {
        if(NULL != m_pBtnCancel)
        {
            m_pBtnOk->show(false);
        }
        
    }
    
    if(style & STYLE_CANCEL)
    {
        if(NULL != m_pBtnCancel)
        {
            m_pBtnCancel->show(true);
            m_pBtnCancel->setPosition(m_ptCancel);
        }
        
        if(!(style& STYLE_OK))
        {
            m_pBtnCancel->setPosition(ccp((m_ptOk.x + m_ptCancel.x)*0.5f, (m_ptOk.y + m_ptCancel.y)*0.5f));
        }
    }
    else
    {
        if(NULL != m_pBtnCancel)
        {
            m_pBtnCancel->show(false);
        }
    }
    
	if(m_pManager != NULL)
    {
        m_pManager->EnableControls(false);
    }
    
	setVisible(true);

	if(waitTimer > 0)
	{
		if(NULL == pAction)
		{
			runAction(CCSequence::createWithTwoActions(CCDelayTime::create(waitTimer),
				CCCallFunc::create(this, callfunc_selector(CCocosTipBox::OnTipFinish))));
		}
		else
		{
			runAction(CCSequence::create(CCDelayTime::create(waitTimer),
				CCCallFunc::create(this, callfunc_selector(CCocosTipBox::OnTipFinish)),
				pAction,
				NULL));
		}
	}
    
    onBeginShow();
}

//----------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::OnTipFinish()
{
	if(m_pManager != NULL)
    {
        m_pManager->EnableControls(true);
    }

	setVisible(false);
    
    onFinishShow();
}

//----------------------------------------------------------------------------------------------------------------------------/
void CCocosTipBox::onReceiveMsg(int aID, int event, const void *pData)
{
    if(NULL != m_pListener)
    {
        m_pListener->onReceiveMsg(m_id, aID);
    }
}



