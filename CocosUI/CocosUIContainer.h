////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CocosUIContainer.h
//
// UI容器，作为父类被继承
//
//                                            By Wander 2012.04.17
/////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CocosButton.h"
#include "CocosSlide.h"

class CCocosUIContainer : public CCocosUIBase
{
public:

	virtual void setEnable(bool bEnable);
	virtual void loseFocus();
	virtual void show(bool bShow);
    virtual void fadeOut(float duration);
    virtual void fadeIn(float duration);
    
    
protected:

    void AddContorl(CCocosUIBase *pNewControl, cocos2d::CCPoint ptPos, int zOrder);
    
	CCocosButton           *AddButton(cocos2d::CCPoint ptPos, int zOrder, int id, CUIListener *pListener,
                                      CCocosButton::ButtonType type = CCocosButton::TYPE_SCALE,
									  bool bUseFrame = false, const char *szTexture1 = NULL, const char *szTexture2 = NULL);
	CCocosSlide            *AddSlide(cocos2d::CCPoint ptPos, int zOrder, int id, CUIListener *pListener,
                                     bool bUseFrame, const char *szBarName,
									 const char *szBtnName = NULL , const char *szBackName = NULL);

	cocos2d::CCSprite      *AddSprite(cocos2d::CCPoint ptPos, int zOrder,bool bUseFrame ,const char *szImage);

	cocos2d::CCLabelBMFont *AddBMText(cocos2d::CCPoint ptPos, int zOrder, const char *szFontFile,
                                      const char *szText, cocos2d::CCPoint ptAnchor = ccp(0, 1));

	

protected:
	ControlList m_listControls;
};
