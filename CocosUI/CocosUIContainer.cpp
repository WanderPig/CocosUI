#include "CocosUIContainer.h"

using namespace std;
using namespace cocos2d;


void CCocosUIContainer::setEnable(bool bEnable) 
{
	CCocosUIBase::setEnable(bEnable);

	for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
	{
		(*iter)->setEnable(bEnable);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosUIContainer::loseFocus()			 
{
	CCocosUIBase::loseFocus();

	for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
	{
		(*iter)->loseFocus();
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosUIContainer::show(bool bShow) 
{
	CCocosUIBase::show(bShow);

	for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
	{
		(*iter)->show(bShow);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosUIContainer::fadeOut(float duration)
{
    for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
    {
        (*iter)->fadeOut(duration);
    }
    
    CCocosUIBase::fadeOut(duration);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosUIContainer::fadeIn(float duration)
{
    for(ControlList::iterator iter = m_listControls.begin(); iter != m_listControls.end(); iter++)
    {
        (*iter)->fadeIn(duration);
    }
    
    CCocosUIBase::fadeIn(duration);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosUIContainer::AddContorl(CCocosUIBase *pNewControl, cocos2d::CCPoint ptPos, int zOrder)
{
	pNewControl->setPosition(ptPos);
	addChild(pNewControl, zOrder);
	m_listControls.push_back(pNewControl);
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosButton *CCocosUIContainer::AddButton(cocos2d::CCPoint ptPos, int zOrder, int id, CUIListener *pListener, CCocosButton::ButtonType type /* = TYPE_SCALE */, 
										   bool bUseFrame /* = false */, const char *szTexture1 /* = NULL */, const char *szTexture2 /* = NULL */)
{
	CCocosButton *pNewBtn = CCocosButton::Create(id, pListener, type, bUseFrame, szTexture1, szTexture2);
	AddContorl(pNewBtn, ptPos, zOrder);
	return pNewBtn;
}

//------------------------------------------------------------------------------------------------------------------------------------/
CCocosSlide *CCocosUIContainer::AddSlide(cocos2d::CCPoint ptPos, int zOrder, int id, CUIListener *pListener, bool bUseFrame, const char *szBarName, 
										 const char *szBtnName /* = NULL  */, const char *szBackName /* = NULL */)
{
	CCocosSlide *pNewSlide = CCocosSlide::Create(id, pListener, bUseFrame, szBarName, szBtnName, szBackName);
	AddContorl(pNewSlide, ptPos, zOrder);
	return pNewSlide;
}

//------------------------------------------------------------------------------------------------------------------------------------/
cocos2d::CCSprite *CCocosUIContainer::AddSprite(cocos2d::CCPoint ptPos, int zOrder, bool bUseFrame, const char *szImage)
{
	CCSprite *pNewSprite;

	if(bUseFrame)
	{
		pNewSprite = CCSprite::createWithSpriteFrameName(szImage);
	}
	else
	{
		pNewSprite = CCSprite::create(szImage);
	}

	pNewSprite->setPosition(ptPos);
	addChild(pNewSprite, zOrder);
	return pNewSprite;
}

//------------------------------------------------------------------------------------------------------------------------------------/
cocos2d::CCLabelBMFont *CCocosUIContainer::AddBMText(cocos2d::CCPoint ptPos, int zOrder, const char *szFontFile, const char *szText, 
													 cocos2d::CCPoint ptAnchor /* = ccp */)
{
	CCLabelBMFont *pFont = CCLabelBMFont::create(szText, szFontFile);
	pFont->setPosition(ptPos);
	pFont->setAnchorPoint(ptAnchor);
	addChild(pFont, zOrder);
	return pFont;
}

