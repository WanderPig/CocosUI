//
//  CocosBtnPopup.cpp
//  Archer2
//
//  Created by Wu Wang on 12-7-5.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CocosBtnPopup.h"

CCocosBtnPopup *CCocosBtnPopup::Create(int id, CUIListener *pListener, bool bUseFrame , const char *szTexture1 , const char *szTexture2 )
{
    CCocosBtnPopup *pNewPopup = CCocosBtnPopup::create();
    pNewPopup->InitState(id, pListener, bUseFrame, szTexture1, szTexture2);
    return pNewPopup;
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosBtnPopup::InitState(int id, CUIListener *pListener, bool bUseFrame , const char *szTexture1 , const char *szTexture2 )
{
    SetData(id, pListener, true);
    m_pBtn = CCocosButton::Create(0, this ,CCocosButton::TYPE_CALLCHANGE, bUseFrame, szTexture1, szTexture2);
    addChild(m_pBtn);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosBtnPopup::onReceiveMsg(int aID, int event, const void *pData)
{
    if(!m_bEnable)
    {
        return;
    }
    
    Popup(!m_bPoping);
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosBtnPopup::setEnable(bool bEnable)
{
    CCocosUIBase::setEnable(bEnable);
    if(NULL != m_pPopMenu)
    {
        m_pPopMenu->setEnable(bEnable);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------/
void CCocosBtnPopup::Popup(bool bPopup)
{
    if(m_bPoping == bPopup)
    {
        return;
    }
    
    PopUpEvent event;
    if(bPopup)
    {
        
        m_pBtn->ShowRelpace();
        event = POPBTN_POP;
    }
    else
    {
        m_pBtn->ShowNormal();
        event = POPBTN_UNPOP;
    }
    
    if(NULL != m_pPopMenu)
    {
        m_pPopMenu->pop(bPopup);
    }
    
    
    m_bPoping = bPopup;
    
    
    m_pListener->onReceiveMsg(m_id, event);
}

