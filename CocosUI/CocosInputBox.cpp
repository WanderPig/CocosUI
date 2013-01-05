#include "CocosInputBox.h"

using namespace cocos2d;

CCocosInputBox *g_pAttachingBox = NULL;

CCocosInputBox::CCocosInputBox()
: m_pSprBack(NULL)
, m_pTFInput(NULL)
, m_bAttaching(false)
, m_cPswMask('*')
, m_startIndex(0)
, m_pLabelMultiLine(NULL)
, m_pTrackNode(NULL)
, m_trackSpace(0)
, m_lastAdjustVertY(0)
{
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::onReceiveNewTouch()
{
	// 获取到一个新的触摸信息
	if(NULL != m_pTFInput && g_pAttachingBox != this)
	{
        
        if(g_pAttachingBox != NULL)
        {
            g_pAttachingBox->SetDetach();
        }
        
		m_pTFInput->attachWithIME();
        g_pAttachingBox = this;
        
        if(NULL == m_pLabelMultiLine)
        {
            schedule(schedule_selector(CCocosInputBox::OnUpdate));
            m_pLableCaret->setPosition(ccp(m_pTFInput->getPosition().x + m_pTFInput->getContentSize().width, m_pTFInput->getPosition().y));
            m_pLableCaret->setVisible(true);
        }
        
        float AdjustVertY = 0;
        if(NULL != m_pTrackNode)
        {
            CCRect rcTrack;
            rcTrack.origin = getParent()->convertToWorldSpace(getPosition());
            rcTrack.size   = getContentSize();
            rcTrack.origin.y =rcTrack.origin.y - rcTrack.size.height*0.5f;
            AdjustVertY = m_boardInfo.end.getMaxY()  - rcTrack.getMinY()  + m_trackSpace+10;
            if(AdjustVertY > 0)
            {
                m_pTrackNode->setPosition(ccp(m_pTrackNode->getPosition().x, m_pTrackNode->getPosition().y + AdjustVertY));
                m_lastAdjustVertY += AdjustVertY;
            }
        }
		
		m_bAttaching = true;
		m_pListener->onReceiveMsg(m_id, EVENT_KEYBOARDSHOW, &AdjustVertY);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
bool CCocosInputBox::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
	if(sender != m_pTFInput)
	{
		return CCTextFieldDelegate::onTextFieldInsertText(sender, text, nLen);
	}
    
	if(m_strBackup.size() >= m_nCharLimit &&  m_nCharLimit > 0)
	{
		return true;
	}
    
	if(*text == '\n')
	{
		return false;
	}
    
	if(m_allowType == ALLOW_NUMONLY)
	{
		if(!(*text >= '0' && *text <= '9') || nLen > 1)
		{
			return true;
		}
	}
	else if(m_allowType == ALLOW_CHARONLY)
	{
		if(nLen > 1)
		{
			return true;
		}
	}
    
	sender->setString(text);
    
	if(sender->getContentSize().width < m_boxSize.width)
	{
		// 如果一次输入过多字符，则不显示，否则当前框没有文字，无法执行删除工作。

        
		m_strBackup  += text;
		// 记录当前长度
		m_vecSubLens.push_back(nLen);
	}
    
	int i = 0;
	int startPos = 0;
	for(i = 0 ; i<m_startIndex; i++)
	{
		startPos += m_vecSubLens[i];
	}
    
	std::string strShow;	// 用于显示的
    
	if(m_showType == TYPE_PASSWORD)
	{
		strShow = std::string(m_vecSubLens.size() - m_startIndex, m_cPswMask);
	}
	else
	{
		strShow = m_strBackup.substr(startPos, m_strBackup.size() - startPos);
	}
    
	// 如果超过框范围，自动左移
	while(true)
	{
		sender->setString(strShow.c_str());
        
		if(sender->getContentSize().width < m_boxSize.width || i == m_vecSubLens.size())
		{
			break;
		}
        
		startPos += m_vecSubLens[i];
        
		if(m_showType == TYPE_PASSWORD)
		{
			strShow = std::string(m_vecSubLens.size() - i, m_cPswMask);
		}
		else
		{
			strShow = m_strBackup.substr(startPos, m_strBackup.size() - startPos);
		}
		i++;
	}
    
	m_startIndex = i;
	m_pLableCaret->setPosition(ccp(m_pTFInput->getPosition().x + m_pTFInput->getContentSize().width , m_pTFInput->getPosition().y));
    
    if(NULL != m_pLabelMultiLine)
    {
        m_pLabelMultiLine->setString(m_strBackup.c_str());
    }
	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------/
bool CCocosInputBox::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
	if(sender != m_pTFInput)
	{
		return CCTextFieldDelegate::onTextFieldDeleteBackward(sender, delText, nLen);
	}
    
	if(m_strBackup.size() > 0)
	{
		m_strBackup.erase(m_strBackup.size() - nLen, m_strBackup.size());
	}
    
    
	int lastLen = m_vecSubLens[m_vecSubLens.size() - 1];
    
	lastLen -= nLen;
    
	m_vecSubLens.erase(m_vecSubLens.end() - 1);
    
	if(lastLen > 0)
	{
		// 还有残留，再压回去
		m_vecSubLens.push_back(lastLen);
	}
    
	if(m_startIndex > 0)
	{
		m_startIndex--;
	}
    
    
	
	float startPos = 0;
    
	for(int i=0; i<m_startIndex; i++)
	{
		startPos += m_vecSubLens[i];
	}
    
	std::string strShow;
    
	if(m_showType == TYPE_PASSWORD)
	{
		strShow = std::string(m_vecSubLens.size() - m_startIndex, m_cPswMask);
	}
	else
	{
		strShow = m_strBackup.substr(startPos, m_strBackup.size() - startPos);
	}
    
	sender->setString(strShow.c_str());
    
	if(sender->getContentSize().width > m_boxSize.width)
	{
		if(m_vecSubLens.size() - m_startIndex> 0)
		{
			startPos += m_vecSubLens[m_startIndex];
			m_startIndex++;
            
			if(m_showType == TYPE_PASSWORD)
			{
				strShow = std::string(m_vecSubLens.size() - m_startIndex, m_cPswMask);
			}
			else
			{
				strShow = m_strBackup.substr(startPos, m_strBackup.size() - startPos);
			}
			sender->setString(strShow.c_str());
		}
	}
    
    if(NULL != m_pLabelMultiLine)
    {
        m_pLabelMultiLine->setString(m_strBackup.c_str());
    }
    
	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info)
{
	memcpy((char *)&m_boardInfo, (char *)&info, sizeof(info));
    m_pCurTouch = NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& info)
{
	if(NULL != m_pTFInput && m_bAttaching)
	{
		SetDetach();
        
        float lastAdjustY = 0;
        if(NULL != m_pTrackNode)
        {
            lastAdjustY = m_lastAdjustVertY;
            m_pTrackNode->setPosition(ccp(m_pTrackNode->getPosition().x, m_pTrackNode->getPosition().y - m_lastAdjustVertY));
            m_lastAdjustVertY = 0;
        }
        
		m_pListener->onReceiveMsg(m_id, EVENT_KEYBOARDHIDE, &lastAdjustY);
	}
    
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::onExit()
{
    SetDetach();
    
    CCocosUIBase::onExit();
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosInputBox *CCocosInputBox::Create(int id, CUIListener *pListener, const char *szHolder, float fontSize, const char *szFontName,
                                       bool bUseFrame, const char *szTexture, const cocos2d::CCSize boxSize)
{
	CCocosInputBox *pNewBox = CCocosInputBox::create();
	pNewBox->InitState(id,pListener, szHolder, fontSize, szFontName, bUseFrame, szTexture, boxSize);
	return pNewBox;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::InitState(int id, CUIListener *pListener, const char *szHolder, float fontSize, const char *szFontName, bool bUseFrame, 
                               const char *szTexture, const cocos2d::CCSize boxSize)
{
	CCocosUIBase::SetData(id, pListener, true);
    
    if(NULL != szTexture)
    {
        if(bUseFrame)
        {
            m_pSprBack = CCSprite::createWithSpriteFrameName(szTexture);
            addChild(m_pSprBack);
        }
        else
        {
            m_pSprBack = CCSprite::create(szTexture);
            addChild(m_pSprBack);
        }
        
        addChild(m_pSprBack);
    }
	
	CCSize size = boxSize;
    
	if(size.equals(CCSizeZero) && m_pSprBack != NULL)
	{
		size = m_pSprBack->getContentSize();
	}
	//m_pTFInput = CCTextFieldTTF::textFieldWithPlaceHolder(szHolder, size, CCTextAlignmentLeft, szFontName, fontSize);
    
    if(fontSize == 0)
    {
        fontSize = size.height;
    }
	m_pTFInput = CCTextFieldTTF::textFieldWithPlaceHolder(szHolder, szFontName, fontSize);
	m_pTFInput->setAnchorPoint(ccp(0, 0.5));
	m_pTFInput->setPosition(ccp(-size.width*0.5f, 0));
	m_pTFInput->setDelegate(this);
    
	m_pLableCaret = CCLabelTTF::create("|","Thonburi", size.height * 0.9f);
	m_pLableCaret->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCFadeIn::create(0.3f),CCDelayTime::create(0.4), CCFadeOut::create(0.3f), NULL)));
	m_pLableCaret->setVisible(false);
    
	m_pLableCaret->setPosition(ccp(m_pTFInput->getPosition().x + m_pTFInput->getContentSize().width, m_pTFInput->getPosition().y));
	setContentSize(size);
	m_boxSize = size;
    
    
    addChild(m_pTFInput);
    addChild(m_pLableCaret);
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::OnUpdate(float dt)
{
	m_pLableCaret->setPosition(ccp(m_pTFInput->getPosition().x + m_pTFInput->getContentSize().width, m_pTFInput->getPosition().y));
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::EnableMultiLine(float fontSize, const char *szFontName)
{
    if(NULL != m_pTFInput)
    {
        m_pTFInput->setVisible(false);
    }
    
    if(NULL != m_pLableCaret)
    {
        m_pLableCaret->setVisible(false);
    }
    
    m_pLabelMultiLine = CCLabelTTF::create(m_strBackup.c_str(), szFontName, fontSize, getContentSize(), kCCTextAlignmentLeft);
    addChild(m_pLabelMultiLine);
    m_pLabelMultiLine->setPosition(ccp(-getContentSize().width*0.5f, getContentSize().height * 0.5f));
    m_pLabelMultiLine->setAnchorPoint(ccp(0, 1));
    
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosInputBox::SetDetach()
{
    m_pTFInput->detachWithIME();
    
    if(NULL == m_pLabelMultiLine)
    {
        unschedule(schedule_selector(CCocosInputBox::OnUpdate));
        m_pLableCaret->setVisible(false);
    }
    
    m_bAttaching = false;
    
    if(g_pAttachingBox == this)
    {
        g_pAttachingBox = NULL;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
const char *CCocosInputBox::GetText()
{
	if(NULL != m_pTFInput)
	{
		return m_strBackup.c_str();
	}
    
	return NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------/
unsigned long CCocosInputBox::GetTextLen()
{
    if(NULL != m_pTFInput)
    {
        return m_strBackup.size();
    }
    
    return 0;
}

void CCocosInputBox::SetText(std::string strText)
{
    m_strBackup = strText;
    
    if(NULL != m_pLabelMultiLine)
    {
        m_pLabelMultiLine->setString(m_strBackup.c_str());
    }
    else if(NULL != m_pTFInput)
    {
        m_pTFInput->setString(m_strBackup.c_str());
    }
}
